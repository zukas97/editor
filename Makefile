CC = gcc
CFLAGS = -Wall -Wextra -ggdb
ARGS = ./src/main.c

SRC = $(shell find ./src/ -type f -name "*.c")
OUT = $(join out/,$(notdir $(SRC:.c=.o)))
INC =
LIB =
LNK = -lncurses -ltinfo
APP = ed

# Directories to search for files
VPATH = .:src

out/%.o : %.c
	$(CC) $(CFLAGS) $(INC) $(LIB) -c $^ -o $@ $(LNK)

$(APP): $(OUT)
	$(CC) $(CFLAGS) $(INC) $(LIB) $^ -o $@ $(LNK)

clean:
	rm $(APP) $(OUT)/*

run: $(APP)
	./$(APP) $(ARGS)

debug: $(APP)
	gdb $(APP) $(ARGS)
