CC=clang
BIN=machine

all:
	$(CC) main.c machine.c -o $(BIN)

run: all
	./$(BIN)

clean:
	rm $(BIN)
	rm mem_dump.txt
