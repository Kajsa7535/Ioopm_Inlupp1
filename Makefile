hash_table: hash_table.c
	gcc -g -pedantic -Wall hash_table.c -lcunit

hash_table_test: hash_table_tests.c
	gcc -g -pedantic -Wall hash_table_tests.c -o $@ -lcunit

test: hash_table_test
	./hash_table_test