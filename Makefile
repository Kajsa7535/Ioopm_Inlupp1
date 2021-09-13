hash_table: hash_table.c
	gcc -g -pedantic -Wall -lcunit hash_table.c 

hash_table_test: hash_table_tests.c
	gcc -g -pedantic -Wall -lcunit hash_table_tests.c -o $@

test: hash_table_test
	./hash_table_test