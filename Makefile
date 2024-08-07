build:
	gcc ./src/main.c -Wall -lncurses -ltinfo -o ed
run:
	make
	./ed
	rm ed
