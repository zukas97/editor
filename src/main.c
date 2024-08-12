#include <assert.h>
#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>

#define ERROR(cond, msg) assert((printf("%s", (cond) ? msg : "\0"), cond))

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

void init(char *filename, bool *running) {
    clear();
    char buffer[1024];
    FILE *fptr;
    fptr = fopen(filename, "r");
    while (fgets(buffer, sizeof(buffer), fptr)) {
        printw(buffer, 0, 0);
    }
    fclose(fptr);
    *running = true;
}

void writeToFile(char *filename) {
    char str[1024];
    FILE *file;
    file = fopen(filename, "w");
    //ERROR(file, "file open failed\n");
    //fseek(file, 0, SEEK_SET);
        str[1023] = '\0';
    for (int i = 0; i < LINES; i++) {
        move(i, 0);
        instr(str);
        fprintf(file, "%s\n", str);
    }
    fclose(file);
}

void charInput(char ch, vec2_t *cursor, mode_t *mode, char *filename, bool *running) {
    int c = inch();
    switch (*mode) {
    case NORMAL:
        switch (ch) {
        case 'q':
            *running = false;
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
            break;
        case 'w':
            writeToFile(filename);
            break;
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
		case 7:
		    // this is backspace
		    mvdelch(cursor->y, cursor->x - 1);
		    cursor->x -= 1;
		    refresh();
		    break;
		case 10:
		    mvprintw(cursor->y, cursor->x,"\n");
		    //cursor->y += 1;
		default:
		    //printf("%d\n", ch);
		    if (c != ' ') {
			mvinsch(cursor->y, cursor->x, ch);
			cursor->x += 1;
			refresh();
		    }
		    else {
			    mvprintw(cursor->y, cursor->x, "%c", ch);
			    cursor->x += 1;
		    }
		    break;
        }
        break;
    }
}

int main(int argc, char **argv) {
    bool running = false;
    vec2_t cursor = {0, 0};
    mode_t mode = NORMAL;
    initscr();
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }
    init(argv[1], &running);
    keypad(stdscr, true);
    cbreak();
    noecho();
    nonl();
    curs_set(0);

    while (running) {
        int ch = getch();
        attroff(A_REVERSE);
        mvprintw(cursor.y, cursor.x, "%c", mvinch(cursor.y, cursor.x));
        charInput(ch, &cursor, &mode, argv[1], &running);
        attron(A_REVERSE);
        mvprintw(cursor.y, cursor.x, "%c", mvinch(cursor.y, cursor.x));
        attroff(A_REVERSE);
        ch = 0;
    }
    endwin();
}
