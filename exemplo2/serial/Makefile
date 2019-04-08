# CC=gcc
FLAGS=-O3 -Wall
DEBUGFLAGS=-g -DDEBUG
TEST=-DTEST
RM=rm -f

EXEC=ACO

.PHONY : all clean debug

all: $(EXEC).c
	$(CC) $(FLAGS) $(EXEC).c -o $(EXEC)

debug: $(EXEC).c
	$(CC) $(FLAGS) $(DEBUGFLAGS) $(EXEC).c -o $(EXEC)

test: $(EXEC).c
	$(CC) $(FLAGS) $(TEST) $(EXEC).c -o $(EXEC)

debugtest: $(EXEC).c
	$(CC) $(FLAGS) $(TEST) $(DEBUGFLAGS) $(EXEC).c -o $(EXEC)

clean:
	$(RM) $(EXEC).o $(EXEC)
