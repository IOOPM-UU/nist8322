CC = gcc
CFLAGS = -Wall -pedantic -g
LDLIBS = -lcunit

.PHONY: test memtest clean coverage

# Kompilera länkade listans tester
list_tests: linked_list.c list_iterator.c linked_list_test.c
	$(CC) $(CFLAGS) linked_list.c list_iterator.c linked_list_test.c $(LDLIBS) -o list_tests

# Kompilera hash-tabellens tester
ht_tests: hash_table.c hash_table_iterator.c hash_table_tests.c
	$(CC) $(CFLAGS) hash_table.c hash_table_iterator.c hash_table_tests.c $(LDLIBS) -o ht_tests

# Kompilera huvudprogrammet freq_count
freq_count: freq_count.c hash_table.c hash_table_iterator.c linked_list.c list_iterator.c
	$(CC) $(CFLAGS) freq_count.c hash_table.c hash_table_iterator.c linked_list.c list_iterator.c -o freq_count

# Kör testerna
test: list_tests ht_tests
	./list_tests
	./ht_tests

# Kör testerna genom Valgrind
memtest: list_tests ht_tests
	valgrind --leak-check=full ./list_tests
	valgrind --leak-check=full ./ht_tests

# Städa upp (ta bort de kompilerade filerna)
clean:
	rm -f list_tests ht_tests freq_count *.o *.gcno *.gcda *.gcov

# Att mäta kodtäckning
coverage:
	$(CC) $(CFLAGS) --coverage linked_list.c list_iterator.c linked_list_test.c $(LDLIBS) -o list_tests
	$(CC) $(CFLAGS) --coverage hash_table.c hash_table_iterator.c hash_table_tests.c $(LDLIBS) -o ht_tests
	./list_tests
	./ht_tests
	gcov -b list_tests-linked_list.gcno list_tests-list_iterator.gcno
	gcov -b ht_tests-hash_table.gcno ht_tests-hash_table_iterator.gcno
