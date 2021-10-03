STD_FLAGS = -g -pedantic -Wall -c
STD_MAIN_FLAGS = -g -pedantic -Wall
TEST_FLAGS = -g -pedantic -Wall -lcunit
GCOV_FLAGS = -Wall -fprofile-arcs -ftest-coverage -lcunit
MEMTEST_FLAGS = --leak-check=full


hash_table: hash_table.c linked_list.c
	gcc hash_table.c linked_list.c $(STD_FLAGS)

ht_tests: hash_table_tests.c hash_table
	gcc hash_table_tests.c hash_table.c linked_list.c $(TEST_FLAGS) 

test_ht: ht_tests
	./a.out

memtest_ht: ht_tests
	valgrind $(MEMTEST_FLAGS) ./a.out

gcov_ht: ht_tests
	gcc $(GCOV_FLAGS) hash_table_tests.c hash_table.c linked_list.c

linked_list: linked_list.c
	gcc linked_list.c $(STD_FLAGS)
	
linked_list_tests: linked_list_tests.c linked_list.c
	gcc linked_list_tests.c linked_list.c $(TEST_FLAGS)

test_linked_list: linked_list_tests
	./a.out

memtest_linked_list: linked_list_tests
	valgrind $(MEMTEST_FLAGS) ./a.out

freq-count: freq-count.c hash_table.c linked_list.c
	gcc freq-count.c hash_table.c linked_list.c $(STD_MAIN_FLAGS) -o freq-count

memtest_freq-count: freq-count
	valgrind $(MEMTEST_FLAGS) ./a.out 10k-words.txt

build_freq-count: freq-count.c hash_table.c linked_list.c
	gcc -pg hash_table.c linked_list.c freq-count.c

run_freq-count: build_freq-count
	./a.out 16k-words.txt

all: ht_tests linked_list_tests

test: test_linked_list test_ht

memtest: memtest_ht memtest_linked_list memtest_freq-count

clean:
	rm -f *.o
	rm -f hash_table ht_tests memtest_ht gcov_ht linked_list linked_list_tests memtest_linked_list freq-count memtest_freq-count

.PHONY: test_ht test_linked_list clean
