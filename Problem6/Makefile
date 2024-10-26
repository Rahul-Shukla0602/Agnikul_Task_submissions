CC = gcc
CFLAGS = -Wall -pthread

all: main worker verifier

main: main.c shared.h
	$(CC) $(CFLAGS) -o main main.c

worker: worker.c shared.h
	$(CC) $(CFLAGS) -o worker worker.c

verifier: verifier.c shared.h
	$(CC) $(CFLAGS) -o verifier verifier.c

clean:
	rm -f main worker verifier

	