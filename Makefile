CC=g++
LIBS=-lraylib
INCLUDE=-I.
OUTPUT=bin

all:
	$(CC) $(LIBS) $(INCLUDE) -o $(OUTPUT) main.cpp

clean:
	rm -f bin
