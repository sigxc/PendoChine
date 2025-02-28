CC = clang
BIN = pcore
FLAGS = -O3 -march=native -mtune=native
FILES = main.c core.c opcodes.c machine.c

all:
	$(CC) $(FLAGS) $(FILES) -o $(BIN)

debug:
	$(CC) -g $(FILES) -o $(BIN)_debug

run: all
	./$(BIN)

clean:
	rm $(BIN)*
	rm mem_dump*
