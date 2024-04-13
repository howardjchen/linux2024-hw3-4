CC = gcc
CFLAGS = -std=c11

EXEC1 = i_sqrt
EXEC2 = ilog2

.PHONY: all clean

all: $(EXEC1) $(EXEC2)

$(EXEC1): i_sqrt.o
	$(CC) $(CFLAGS) i_sqrt.o -o $(EXEC1)

$(EXEC2): ilog2.o
	$(CC) $(CFLAGS) ilog2.o -o $(EXEC2)

i_sqrt.o: i_sqrt.c
	$(CC) $(CFLAGS) -c i_sqrt.c -o i_sqrt.o

ilog2.o: ilog2.c
	$(CC) $(CFLAGS) -c ilog2.c -o ilog2.o

clean:
	rm -f i_sqrt.o ilog2.o $(EXEC1) $(EXEC2)
