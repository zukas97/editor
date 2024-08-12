CC = gcc
CFLAGS = -Wall -Wextra -ggdb
# Args to call the app with
ARGS = ./src/main.c

# Source files
SRC = $(shell find ./src/ -type f -name "*.c")
# Object files. Takes the raw filename from SRC, replaces the .c with .o, and joins it with out/
# So foo/bar/baz.c => out/baz.o
OUT = $(join out/,$(notdir $(SRC:.c=.o)))
# Include directories
INC =
# Libary directories
LIB =
# Links and shared objects
LNK = -lncurses -ltinfo
# The app name
APP = ed
###############################################################################

# Directories to search for files
VPATH = .:src

###############################################################################
# A recipe to make any out file, searched for in src path
out/%.o : %.c
	$(CC) $(CFLAGS) $(INC) $(LIB) -c $^ -o $@ $(LNK)

# A recipe to make the app
$(APP): $(OUT)
	$(CC) $(CFLAGS) $(INC) $(LIB) $^ -o $@ $(LNK)

clean:
	rm $(APP) $(OUT)/*

# `run` depends on `app` to be built. If it isnt built, it will build it
# If it is, it wont. After that it will call ./app with the arguments
run: $(APP)
	./$(APP) $(ARGS)

debug: $(APP)
	gdb $(APP) $(ARGS)
