CC=gcc

OBJECTS=pqueue.o

CFLAGS=-Wall -Wextra -g

all: pqueuef_test

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -c

%_test: $(OBJECTS) %_test.c
	$(CC) $(CFLAGS) $^ -lcunit -o $@

test: pqueue_test
	./pqueue_test

memtest: pqueue_test
	valgrind --leak-check=full ./pqueue_test

clean:
	rm -f pqueue_test
	rm -f *.o
	rm -rf doc

.PHONY: test clean all
