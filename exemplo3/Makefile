# CC=gcc
FLAGS=-O3 -lm -fopenmp
DEBUGFLAGS=-g
RM=rm -f

EXEC=ACA

.PHONY : all clean debug

all: $(EXEC).c
	$(CC) $(FLAGS) $(EXEC).c -o $(EXEC)

debug: $(EXEC).c
	$(CXX) $(FLAGS) $(DEBUGFLAGS) $(EXEC).c -o $(EXEC)

clean:
	$(RM) $(EXEC).o $(EXEC)
