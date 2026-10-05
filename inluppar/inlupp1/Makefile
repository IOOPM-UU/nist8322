# Variabler för att göra det enkelt att ändra senare
CC = gcc
CFLAGS = -Wall -pedantic -g
LDLIBS = -lcunit

# Regel 1: Kompilera länkade listans tester
list_tests: linked_list.c list_iterator.c linked_list_test.c
	$(CC) $(CFLAGS) linked_list.c list_iterator.c linked_list_test.c $(LDLIBS) -o list_tests

# Regel 2: Kompilera hash-tabellens tester (som vi jobbade med nyss)
ht_tests: hash_table.c hash_table_iterator.c hash_table_tests.c
	$(CC) $(CFLAGS) hash_table.c hash_table_iterator.c hash_table_tests.c $(LDLIBS) -o ht_tests

# Regel 3: Kompilera huvudprogrammet freq_count (här behövs inte -lcunit, men filerna för datastrukturerna måste vara med)
freq_count: freq_count.c hash_table.c hash_table_iterator.c linked_list.c list_iterator.c
	$(CC) $(CFLAGS) freq_count.c hash_table.c hash_table_iterator.c linked_list.c list_iterator.c -o freq_count

# Regel 4: Kör testerna
test: list_tests ht_tests
	./list_tests
	./ht_tests

# Regel 5: Kör testerna genom Valgrind
memtest: list_tests ht_tests
	valgrind --leak-check=full ./list_tests
	valgrind --leak-check=full ./ht_tests

# Regel 6: Städa upp (ta bort de kompilerade filerna)
clean:
	rm -f list_tests ht_tests freq_count

# Regel för att mäta kodtäckning
coverage:
	$(CC) $(CFLAGS) --coverage linked_list.c list_iterator.c linked_list_test.c $(LDLIBS) -o list_tests
	$(CC) $(CFLAGS) --coverage hash_table.c hash_table_iterator.c hash_table_tests.c $(LDLIBS) -o ht_tests
	./list_tests
	./ht_tests
	gcov -b list_tests-linked_list.gcno list_tests-list_iterator.gcno
	gcov -b ht_tests-hash_table.gcno ht_tests-hash_table_iterator.gcno