#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

// #define ctrl(x) ((x) & 0x1f)

// utility
typedef struct vec2 {
    uint64_t x;
    uint64_t y;
} vec2_t;

typedef enum mode {
    NORMAL,
    INSERT,
    REPLACE,
    // VISUAL,
    // COMMAND,
} mode_t;

bool running = false;

void init(char *filename) {
    clear();
    char buffer[1024];
    FILE *fptr;
    fptr = fopen(filename, "r");
    while (fgets(buffer, sizeof(buffer), fptr)) {
        printw(buffer, 0, 0);
    }
    fclose(fptr);
    running = true;
}

void write(char *filename, vec2_t *cursor) {
	char buff[1024];
	FILE* file;
	file = fopen(filename, "w");
	for (int i = 0; i < LINES; i++) {
		move(i, 0);
		instr(buff);
		fprintf(file, "%s\n", buff);

	}
	fclose(file);

}

void charInput(char ch, vec2_t *cursor, mode_t *mode, char * filename) {
    int c = inch();
    switch (*mode) {
        case NORMAL:
            switch (ch) {
                case 'q':
                    running = false;
                    break;
                case 'h':
                    cursor->x -= 1;
                    break;
                case 'j':
                    cursor->y += 1;
                    break;
                case 'k':
                    cursor->y -= 1;
                    break;
                case 'l':
                    cursor->x += 1;
                    break;
                case 'R':
                    *mode = REPLACE;
                    break;
                case 'i':
                    *mode = INSERT;
		case 'w':
		    write(filename, cursor);
            }
            break;
        case REPLACE:
            switch (ch) {
                case 27:
                    *mode = NORMAL;
                    break;
                default:
                    mvprintw(cursor->y, cursor->x, "%c", ch);
                    cursor->x += 1;
                    break;
            }
            break;
        case INSERT:
            switch (ch) {
                case 27:
                    *mode = NORMAL;
                    break;
                case 8:
                    mvdelch(cursor->y, cursor->x - 1);
		    cursor->x -= 1;
                    refresh();
		    break;
                case 7:
                    // this is backspace
                    mvdelch(cursor->y, cursor->x - 1);
		    cursor->x -= 1;
                    refresh();
                    break;
                default:
                    printf("%d\n", ch);
                    if (c != ' ') {
                        // cursor->x += 1;
                        mvinsch(cursor->y, cursor->x, ch);
                        refresh();
                    }
                    mvprintw(cursor->y, cursor->x, "%c", ch);
                    cursor->x += 1;
                    break;
            }
            break;
    }
}

int main(int argc, char **argv) {
    // printf("hello\n");
    // char *arg = *argv;
    vec2_t cursor = {0, 0};
    mode_t mode = NORMAL;
    initscr();
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }
    init(argv[1]);
    keypad(stdscr, true);
    cbreak();
    noecho();
    nonl();
    curs_set(0);

    while (running) {
        int ch = getch();
        // addch(ch);
        attroff(A_REVERSE);
        mvprintw(cursor.y, cursor.x, "%c", mvinch(cursor.y, cursor.x));
        charInput(ch, &cursor, &mode, argv[1]);
        attron(A_REVERSE);
        mvprintw(cursor.y, cursor.x, "%c", mvinch(cursor.y, cursor.x));
        attroff(A_REVERSE);
        ch = 0;
    }
    endwin();
}
