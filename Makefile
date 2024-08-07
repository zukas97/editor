build:
	gcc -ggdb ./src/main.c -Wall -lncurses -ltinfo -o ed
run:
	make
	./ed
	rm ed
debug:
	make
	gdb ed
	rm ed
