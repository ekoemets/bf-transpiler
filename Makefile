CC = gcc
CFLAGS = -std=c99 -g -Wall -Iinclude

PROGRAM = bin/brainfuck

OBJ = obj/main.o obj/list.o obj/bf_node.o obj/parser.o obj/interpretator.o obj/transpiler.o

all: build

build: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(PROGRAM)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -rf obj/*.o $(PROGRAM)
 
