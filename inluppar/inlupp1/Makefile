CC = gcc
CFLAGS = -Wall -pedantic -g
LDLIBS = -lcunit

# De som inte skapar filer
.PHONY: all test memtest clean coverage

all: list_tests ht_tests freq_count

list_tests: linked_list.o list_iterator.o linked_list_test.o
	$(CC) $(CFLAGS) linked_list.o list_iterator.o linked_list_test.o $(LDLIBS) -o list_tests

ht_tests: hash_table.o hash_table_iterator.o hash_table_tests.o
	$(CC) $(CFLAGS) hash_table.o hash_table_iterator.o hash_table_tests.o $(LDLIBS) -o ht_tests

freq_count: freq_count.o hash_table.o hash_table_iterator.o linked_list.o list_iterator.o
	$(CC) $(CFLAGS) freq_count.o hash_table.o hash_table_iterator.o linked_list.o list_iterator.o -o freq_count

test: list_tests ht_tests
	./list_tests
	./ht_tests

memtest: list_tests ht_tests
	valgrind --leak-check=full ./list_tests
	valgrind --leak-check=full ./ht_tests

clean:
	rm -f list_tests ht_tests freq_count *.o *.gcno *.gcda *.gcov

coverage: CFLAGS += --coverage
coverage: clean test
	gcov -b linked_list.c list_iterator.c hash_table.c hash_table_iterator.c