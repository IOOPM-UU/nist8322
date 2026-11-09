CC = gcc
CFLAGS = -Wall -g -Ibusiness_logic -Igeneric_data_structures -Igeneric_utils -Iuser_interface

TARGET = webshop
TEST_TARGET = test_backend

SRCS = main.c db_backend.c db_frontend.c utils.c hash_table.c linked_list.c hash_table_iterator.c list_iterator.c
OBJS = $(SRCS:.c=.o)

TEST_SRCS = db_tests.c db_backend.c utils.c hash_table.c linked_list.c hash_table_iterator.c list_iterator.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

vpath %.c business_logic
vpath %.c generic_data_structures
vpath %.c generic_utils
vpath %.c user_interface

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCDIRS) $^ -o $@

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCDIRS) $^ -o $@ -lcunit

test: $(TEST_TARGET)
	./$(TEST_TARGET)

memtest: $(TEST_TARGET)
	valgrind --leak-check=full ./$(TEST_TARGET)

run_test: $(TARGET)
	./$(TARGET) < tests/webstoretest.txt

run_test2: $(TARGET)
	./$(TARGET) < tests/test2.txt

run_memtest: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) < tests/webstoretest.txt

run: $(TARGET)
	./$(TARGET)

# --- Kodtäckning (Coverage) ---
coverage: CFLAGS += --coverage
coverage: clean $(TEST_TARGET)
	./$(TEST_TARGET)
	lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
	genhtml coverage.info --output-directory coverage_report --rc lcov_branch_coverage=1
	lcov --list coverage.info --rc lcov_branch_coverage=1

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET) db_tests ht_tests list_tests *.gcda *.gcno coverage.info
	rm -rf coverage_report

	