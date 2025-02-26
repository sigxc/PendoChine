CC = clang
BIN = core
FLAGS = -O3 -march=native -mtune=native -lraylib -lm -lpthread -ldl -lrt -lX11
FILES = main.c core.c opcodes.c machine.c

all:
	$(CC) $(FLAGS) $(FILES) -o $(BIN)

debug:
	$(CC) -g $(FILES) -o $(BIN)_debug

verbose:
	$(CC) $(FLAGS) -DVERBOSE $(FILES) -o $(BIN)_verbose

run: all
	./$(BIN)

clean:
	rm $(BIN)*
	rm mem_dump*
