CC = gcc

CFLAGS = -std=gnu89 -Wall
LFLAGS = -lSDL2

SRC = TILEX.c 
OBJ = TILEX 

all : $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LFLAGS) -o $(OBJ)