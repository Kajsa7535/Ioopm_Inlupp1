hash_table: hash_table.c
	gcc -g -pedantic -Wall hash_table.c -o $@

hash_table_tests: hash_table_tests.c 
	gcc -g -pedantic -Wall hash_table.c hash_table_tests.c -o $@ -lcunit

test_hash_table: hash_table_tests
	./hash_table_tests

valgrind_hash_table_tests: hash_table_tests
	valgrind --leak-check=full ./hash_table_tests

gcov_hash_table: hash_table_tests.c
	 gcc -Wall -fprofile-arcs -ftest-coverage hash_table_tests.c hash_table.c -lcunit

linked_list: linked_list.c
	 gcc -g -pedantic -Wall linked_list.c -c -o $@
	
linked_list_tests: linked_list_tests.c 
	gcc -g -pedantic -Wall linked_list.c linked_list_tests.c -o $@ -lcunit

test_linked_list: linked_list_tests
	./linked_list_tests

vvalgrindlinked_liste_tests: linked_list_test
	valgrind --leak-check=full ./linked_list_tests

	 