CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: fs_sim

fs_sim: main.o fs_indexed.o
	$(CC) $(CFLAGS) -o fs_sim main.o fs_indexed.o

main.o: main.c fs_indexed.h
	$(CC) $(CFLAGS) -c main.c

fs_indexed.o: fs_indexed.c fs_indexed.h
	$(CC) $(CFLAGS) -c fs_indexed.c

run: fs_sim
	./fs_sim

clean:
	rm -f *.o fs_sim