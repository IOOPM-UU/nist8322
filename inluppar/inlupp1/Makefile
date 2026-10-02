# Variabler för att göra det enkelt att ändra senare
CC = gcc
CFLAGS = -Wall -pedantic -g
LDLIBS = -lcunit

# Regel 1: Kompilera ihop programmet "tests"
tests: linked_list.c list_iterator.c linked_list_test.c
	$(CC) $(CFLAGS) linked_list.c list_iterator.c linked_list_test.c $(LDLIBS) -o tests

# Regel 2: Kör testerna vanligt
test: tests
	./tests

# Regel 3: Kör testerna genom Valgrind
memtest: tests
	valgrind --leak-check=full ./tests

# Regel 4: Städa upp (ta bort den kompilerade filen)
clean:
	rm -f tests