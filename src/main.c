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

// so we can insert stuff
typedef enum mode {
    NORMAL,
    INSERT,
    REPLACE,
    // VISUAL,
    // COMMAND,
} mode_t;

// dont use global variables lad
// pass by reference or pass by value
// it wont allow you to use the same variable name
// and it causes a little bit much of overhead
bool running = false;
char buffer[1024];
int x;
int y;

void init(char *filename) {
    clear();

    x = 0;
    y = 0;
    FILE *fptr;
    fptr = fopen(filename, "r");
    while (fgets(buffer, sizeof(buffer), fptr)) {
        // buffer doesnt contain a format string so x and y are just deleted
        // your approach is correct though: print the lines
        // printw does that for you already so you can actually remove x, y
        printw(buffer, x, y);
    }
    fclose(fptr);
    running = true;
}

void charInput(char ch, vec2_t *cursor, mode_t *mode) {
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
                    mvdelch(y, x - 1);
                    refresh();
                case 7:
                    // this is backspace
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
        charInput(ch, &cursor, &mode);
        attron(A_REVERSE);
        mvprintw(cursor.y, cursor.x, "%c", mvinch(cursor.y, cursor.x));
        attroff(A_REVERSE);
        ch = 0;
    }
    endwin();
}
