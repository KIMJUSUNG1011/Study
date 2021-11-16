all: CFS CFSnice
CC=gcc
CFLAGS=-Wall

CFS : cfs.c
	$(CC) -o $@ $< $(CFLAGS)

CFSnice : nice.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -r CFS
	rm -r CFSnice
	rm -r a.out
