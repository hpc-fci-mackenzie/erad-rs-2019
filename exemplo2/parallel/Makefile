# CC=gcc
FLAGS=-O3 -Wall -fopenmp
DEBUGFLAGS=-g -DDEBUG
RM=rm -f

EXEC=ACO

.PHONY : all clean debug

all: $(EXEC).c
	$(CC) $(FLAGS) $(EXEC).c -o $(EXEC)

debug: $(EXEC).c
	$(CC) $(FLAGS) $(DEBUGFLAGS) $(EXEC).c -o $(EXEC)

clean:
	$(RM) $(EXEC).o $(EXEC)
