#include <stdio.h>
#include <curses.h>
#include <stdbool.h>
//#include <stdlib.h>
//#include <unistd.h>

bool running = false;
char buffer[1024];
int x;
int y;

void init(char * arg) {
	x = 0;
	y = 0;
	printf("hello\n");
	FILE *fptr;
	fptr = fopen(arg, "r");
	initscr();
	while (fgets(buffer, sizeof(buffer), fptr)) {
		printw(buffer, x, y);
	}
	running = true;
}

int main(int argc, char **argv) {
	//printf("hello\n");
	//initscr();
	char *arg = *argv;
	init(arg);
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
