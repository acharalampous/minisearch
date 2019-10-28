CC = gcc
CFLAGS = -g -Wall
OBJECTS = main.o PostingList.o Trie.o WordList.o DocMap.o acutils.o
MATH = -lm

minisearch: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o minisearch $(MATH)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

PostingList.o: PostingList.c
	$(CC) $(CFLAGS) -c PostingList.c

Trie.o: Trie.c
	$(CC) $(CFLAGS) -c Trie.c

WordList.o: WordList.c
	$(CC) $(CFLAGS) -c WordList.c

DocMap.o: DocMap.c
	$(CC) $(CFLAGS) -c DocMap.c	

acutils.o: acutils.c
	$(CC) $(CFLAGS) -c acutils.c

.PHONY: clean

clean:
	rm -f minisearch $(OBJECTS) 
