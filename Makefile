
BIN = raymarcher

CC = gcc

SDL = /usr/local/Cellar/sdl2/2.0.8

CFLAG = -O3 -ffast-math
SRC = main.c
HDR = 
INC = -I$(SDL)/include
LIB = -L$(SDL)/lib -lSDL2 -lm

default: $(BIN)

$(BIN): $(SRC) $(HDR)
	$(CC) $(CFLAG) -o $@ $(SRC) $(INC) $(LIB)

clean:
	rm -f $(BIN)
