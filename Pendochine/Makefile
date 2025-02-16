CC = clang
BIN = machine
FLAGS = -O3 -march=native -mtune=native

all:
	$(CC) $(FLAGS) core.c machine.c opcodes.c -o $(BIN)

debug:
	$(CC) -g core.c machine.c opcodes.c -o $(BIN)_debug

verbose:
	$(CC) $(FLAGS) -DVERBOSE core.c machine.c opcodes.c -o $(BIN)_verbose

run: all
	./$(BIN)

clean:
	rm $(BIN)
	rm mem_dump.hex
	rm $(BIN)_debug
	rm $(BIN)_verbose
	rm mem_dump.bin
