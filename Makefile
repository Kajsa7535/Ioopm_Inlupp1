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

gcov_build_ht: hash_table.c linked_list.c hash_table_tests.c
	gcc --coverage hash_table.c linked_list.c hash_table_tests.c -lcunit

gcov_run_ht: gcov_build_ht
	./a.out

gcov_ht: gcov_run_ht
	gcov -abcfu hash_table.c

gcov_build_linked_list: linked_list.c linked_list_tests.c
	gcc --coverage linked_list.c linked_list_tests.c -lcunit

gcov_run_linked_list: gcov_build_linked_list
	./a.out

gcov_linked_list: gcov_run_linked_list
	gcov -abcfu linked_list.c

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

memtest_freq-count: build_freq-count
	valgrind --tool=massif ./a.out 3m-words.txt 

build_freq-count: freq-count.c hash_table.c linked_list.c
	gcc -pg hash_table.c linked_list.c freq-count.c

run_freq-count: build_freq-count
	./a.out 3m-words.txt 

gprof_freq-count: run_freq-count
	gprof ./a.out

build_freq-count_old: freq-count_old.c hash_table_old.c linked_list.c
	gcc -pg freq-count_old.c linked_list.c hash_table_old.c 

run_freq-count_old: build_freq-count_old
	./a.out 3m-words.txt

gprof_freq-count_old: run_freq-count_old
	gprof ./a.out

build_freq-count_old2: freq-count_old2.c hash_table_old2.c linked_list.c
	gcc -pg freq-count_old2.c linked_list.c hash_table_old2.c 

run_freq-count_old2: build_freq-count_old2
	./a.out 3m-words.txt

gprof_freq-count_old2: run_freq-count_old2
	gprof ./a.out

memtest_freq-count_old2: build_freq-count_old2
	valgrind --tool=massif ./a.out 3m-words.txt 

all: ht_tests linked_list_tests

test: test_linked_list test_ht

memtest: memtest_ht memtest_linked_list

clean:
	rm -f *.o
	rm -f hash_table ht_tests memtest_ht gcov_ht linked_list linked_list_tests memtest_linked_list freq-count build_freq-count memtest_freq-count gprof_freq-count gprof_freq-count_old gprof_freq-count_old2

.PHONY: test_ht test_linked_list clean
