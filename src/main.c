#include <stdio.h>
#include <curses.h>
#include <stdbool.h>
//#include <stdlib.h>
//#include <unistd.h>

//#define ctrl(x)           ((x) & 0x1f)

bool running = false;
char buffer[1024];
int x;
int y;

void init(char *filename) {
	x = 0;
	y = 0;
	printf("hello\n");
	FILE *fptr;
	fptr = fopen(filename, "r");
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	nonl();
	while (fgets(buffer, sizeof(buffer), fptr)) {
		printw(buffer, x, y);
	}
	fclose(fptr);
	running = true;

}

int main(int argc, char **argv) {
	//printf("hello\n");
	//initscr();
	//char *arg = *argv;
	init(argv[1]);
	while (running) {
		int ch = getch();
		//addch(ch);
		if (ch == 'q') {
			running = false;
		}
		ch = 0;
		endwin();
	}
	endwin();

}
