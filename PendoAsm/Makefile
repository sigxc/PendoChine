CC = clang
BIN = pasm
SRC = pasm.c

all:
	$(CC) $(SRC) -o $(BIN)

run: all
	./$(BIN) file.asm o.bin && xxd o.bin

c:
	rm $(BIN)
	rm o.bin
