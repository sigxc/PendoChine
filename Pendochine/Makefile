CC = clang
BIN = machine
FLAGS = -O3 -march=native -mtune=native -flto -fvectorize -fslp-vectorize

all:
	$(CC) $(FLAGS) main.c machine.c opcodes.c -o $(BIN)

debug:
	$(CC) -g main.c machine.c opcodes.c -o "$(BIN)_debug"

verbose:
	$(CC) $(FLAGS) -DVERBOSE main.c machine.c opcodes.c -o "$(BIN)_verbose"

run: all
	./$(BIN)

clean:
	rm $(BIN)
	rm mem_dump.txt
