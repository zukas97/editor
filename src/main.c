#include <stdio.h>
#include <curses.h>
#include <stdbool.h>
//#include <stdlib.h>
//#include <unistd.h>

bool running = false;

void init() {
	printf("hello\n");
	initscr();
	running = true;
}

int main() {
	//printf("hello\n");
	//initscr();
	init();
	while (running) {
		char ch = getch();
		//addch(ch);
		if (ch == 'q') {
			running = false;
		}
		ch = 0;
		endwin();
	}

}
