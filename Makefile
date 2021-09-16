hash_table: hash_table.c
	gcc -g -pedantic -Wall -c hash_table.c -o $@

hash_table_tests: hash_table_tests.c 
	gcc -g -pedantic -Wall hash_table.c hash_table_tests.c -o  $@ -lcunit

test: hash_table_tests
	./hash_table_tests

valgrind_hash_table_tests: hash_table_tests
	valgrind --leak-check=full ./hash_table_tests
