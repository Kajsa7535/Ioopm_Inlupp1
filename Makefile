STD_FLAGS = -g -pedantic -Wall -c
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
	gcc $(GCOV_FLAGS) $^



linked_list: linked_list.c
	gcc $(STD_FLAGS) $^
	
linked_list_tests: linked_list_tests.c linked_list.c
	gcc $(TEST_FLAGS) $^

test_linked_list: linked_list_tests
	./a.out

memtest_linked_list: linked_list_tests
	valgrind $(MEMTEST_FLAGS) ./a.out

clean:
	rm -f *.o
	rm -f hash_table hash_table_tests valgrind_hash_table_tests gcov_hash_table linked_list linked_list_tests valgrind_linked_list_tests	

.PHONY: test_ht test_linked_list clean
