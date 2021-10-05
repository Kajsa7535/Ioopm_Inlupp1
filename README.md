# Ioopm_Inlupp1

This libary consists of a few different data types, a hash_table (in hash_table.c), a linked list (in linked_list.c) and an iterator (in linked_list.c). Typedef, structs and definitions of public functions have been placed in respective .h files. Common typedefs (between the .c files) have been placed in common.h.

# Build and run

    $ make test # bulid and run all tests
    $ make memtest # run all tests including freq-count through valgrind
    $ make all # build all files for hash_table, linked_list and iterator
    $ make linked_list_tests # build linked_list_tests.c file
    $ make ht_tests # build hash_table_tests.c file
    $ make gcov_ht # runs gcov on hash_table.c and hash_table_tests.c
    $ make gcov_linked_list # runs gcov on linked_list.c and linked_list_tests.c
    $ make clean # removes unneccessary files

# Design Decisions

This library has one major deviation from the specification. In the specification, the function called ioopm_hash_table_lookup should "return the value mapped to by key". In this library, the function called ioopm_hash_table_lookup returns a bool depending of the existence of a key in a hash table and stores its value in a pointer given as a parameter. The new function called ioopm_lookup_key returns the value of the given key, if it exists within the hash table. 

A design decision which has made a noticeable impact on the code and its structure was the optimisation of a double pointer as "buckets" in the hash table structure. This decision has made the removal of sentinel-entries possible. 

This libary does not have any special error handling, it uses a mixture of asserts, "error messages" and preconditions.
The hash table does not take ownership of the data of its keys and values.

# Initial Profiling Results

Vi använde oss av gprof för att få fram resultaten. Först kompilerade vi freq-count med -pg flaggan (gcc -pg hash_table.c linked_list.c freq-count.c) och sedan körde vi filen med de olika textfilerna som argument (./a.out name.txt). Slutligen körde vi gprof på ./a.out-filen (gprof ./a.out).

For each input, what are the top 3 functions? (vi går efter "self seconds", men vi anger antalet calls)

- small.txt: 
    1) string_eq (% time: N/A, self seconds: N/A, calls: 155), 
    2) calculate_bucket (% time: N/A, self seconds: N/A, calls: 81)
    3) find_previuos_entry_for_key(% time: N/A, self seconds: N/A, calls: 81)
    4) string_sum_hash (% time: N/A, self seconds: N/A, calls: 81)
Det går så snabbt för filen att köras att det inte går att räkna tiden som funktionerna körs, därmed måste vi kolla på antalet calls i small.txt. Antalet calls ändras inte för varje "körning" av programmet på filen, så endast en körning behövs.

- 1k-long-words.txt:
    1) string_eq (% time: N/A, self seconds: N/A, calls: 7452), 
    2) calculate_bucket (% time: N/A, self seconds: N/A, calls: 3000)
    3) find_previuos_entry_for_key(% time: N/A, self seconds: N/A, calls: 3000)
    4) string_sum_hash (% time: N/A, self seconds: N/A, calls: 3000)
Det går så snabbt för filen att köras att det inte går att räkna tiden som funktionerna körs, därmed måste vi kolla på antalet calls i 1k-long-words.txt. Antalet calls ändras inte för varje "körning" av programmet på filen, så endast en körning behövs.

- 10k-words.txt: 
    1) string_eq (% time: N/A, self seconds: N/A, calls: 256527)
    2) calculate_bucket (% time: N/A, self seconds: N/A, calls: 30000)
    3) find_previous_entry_for_key (% time: N/A, self seconds: N/A, calls: 30000)
    4) string_sum_hash (% time: N/A, self seconds: N/A, calls: 30000)
Det går så snabbt för filen att köras att det inte går att räkna tiden som funktionerna körs, därmed måste vi kolla på antalet calls i 10k-words.txt. Antalet calls ändras inte för varje "körning" av programmet på filen, så endast en körning behövs.
    
- 16k-words.txt:
    Körning 1:
    1) find_previous_entry_for_key (% time: 50.03, self seconds: 0.05, calls: 50976)
    2) string_eq (% time: 30.02, self seconds: 0.03, calls: 5750209)
    3) string_sum_hash(% time: 10.01,  self seconds: 0.1, calls: 50976)

    Körning 2:
    1) find_previous_entry_for_key (% time: 37.52, self seconds: 0.03, calls: 50976)
    2) string_eq (% time: 18.76, self seconds: 0.02, calls: 5750209)
    3) ioopm_hash_table_lookup(% time: 12.51,  self seconds: 0.01, calls: 33984)
    4) ioopm_hash_table_insert (% time: 12.51, self seconds: 0.01, calls: 16992)
    5) bucket_destroy (% time: 12.51,  self seconds: 0.01, calls: 17)

    Körning 3:
    1) find_previous_entry_for_key (% time: 60.03, self seconds: 0.03, calls: 50976)
    2) bucket_destroy (% time: 12.01,  self seconds: 0.01, calls: 17)
    3) string_eq (% time: 10.01, self seconds: 0.01, calls: 5750209)
    4) linked_list_to_array (% time: 10.01,  self seconds: 0.01, calls: 1)

Vi får olika resultat varje körning. Vi kan dock se att programmet alltid spenderar mest tid på find_previous_entry_for_key och i alla körningar är string_eq en av de funktioner som körs mest. Resten av funktionerna varierar.  

For each input, what are the top 3 functions in your code (that you have written), or is it library functions?
- small: Det är våra funktioner (de som står i punkten ovan) som visas. Vi får inte upp några libary functions
- 1k: Det är våra funktioner (de som står i punkten ovan) som visas. Vi får inte upp några libary functions
- 10k: Det är våra funktioner (de som står i punkten ovan) som visas. Vi får inte upp några libary functions
- 16k: Det är våra funktioner (de som står i punkten ovan) som visas. Vi får inte upp några libary functions

Are the top 3 functions in your code consistent across the inputs? Why? Why not?
- Inte alla, men find_previous_entry_for_key och string_eq är alltid med bland toppfunktionerna. För de mindre filerna var calculate_bucket alltid med bland toppen, men vi förväntar oss inte att den ska ta lång tid att köra, och de mindre filerna jämför bara antalet calls.

Is there some kind of trend? (Possibly several.)
- Funktioner som loopar igenom ett hash_table och funktioner som jämför element är de som tar längst tid att köra. 

Do the results correspond with your expectations?
- Ja, det är väldigt rimliga resultat vi har fått. Att loopa igenom ett hash_table tar O(N) tid, där N är antalet element, och vi ser att dessa typer av funktioner är de som tar längst tid på den större filen 16k-words.txt. 

Based on these results, do you see a way to make your program go faster?
- Att öka antalet buckets skulle göra att det blir färre entries i varje bucket, vilket gör att find_previous_entry_for_key inte skulle behöva lika mycket tid att loopa igenom hela bucketen för att hitta rätt entry. 


# Testing freq-count with different load sizes
Run the program with input of varying sizes, and produce execution profiles for the program runs. What are the 10 most time-consuming functions? Are they consistently the same? Or are there functions that e.g. seem to run a lot slower if there is a little more data? Experiment with various load factors – how does that impact the profiling results in 2.?

Load factor 0.2:

    Load factor too small


Load factor 0.3:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s

    16k-words.txt: 0s


Load factor 0.75:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s

    16k-words.txt: 0s


Load factor 0.9:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s

    16k-words.txt:
        1) string_eq (% time: 33.35, self seconds: 0.01, calls: 88182)
        2) string_knr_hash (% time: 33.35, self seconds: 0.01, calls: 82694)
        3) ioopm_hash_table_lookup (% time: 33.35, self seconds: 0.01, calls: 58334)


Load factor 1.5:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s
    
    16k-words.txt:
        1) ioopm_hash_table_insert (% time: 100.05, self seconds: 0.01, calls: 23119)


Load factor 3.0:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 
        1) ioopm_hash_table_lookup (% time: 100.05, self seconds: 0.01, calls: 30143)
    
    16k-words.txt:
        1) calculate_bucket (% time: 100.05, self seconds: 0.01, calls: 80148)


Load factor 10.0:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s
    
    16k-words.txt: 0s


Load factor 100.0:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 
        1) string_eq (% time: 100.05, self seconds: 0.01, calls: 372955)

    16k-words.txt: 
        1) string_eq (% time: 50.02, self seconds: 0.01, calls: 3267835)
        2) find_previous_entry_for_key_ptr (% time: 50.02, self seconds: 0.01, calls: 12189)


Load factor 1000.0:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s

    16k-words.txt: 
        1) string_eq (% time: 50.02, self seconds: 0.03, calls: 8321796)
        2) ioopm_hash_table_lookup (% time: 33.35, self seconds: 0.02, calls: 50975)
        3) find_previous_entry_for_key_ptr (% time: 16.67, self seconds: 0.01, calls: 12400)


Load factor 10000.0:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s

    16k-words.txt: 
        1) ioopm_hash_table_lookup (% time: 85.75, self seconds: 0.06, calls: 50975)
        2) string_eq (% time: 7.15, self seconds: 0.01, calls: 8321796)
        3) string_knr_hash (% time: 7.15, self seconds: 0.01, calls: 67967)


Load factor 20000.0:

    small.txt: 0s

    1k-long-words.txt: 0s

    10k-words.txt: 0s

    16k-words.txt: 
        1) ioopm_hash_table_lookup (% time: 36.38, self seconds: 0.04, calls: 50975)
        2) string_eq (% time: 31.83, self seconds: 0.04, calls: 8321796)
        3) find_previous_entry_for_key (% time: 18.19, self seconds: 0.02, calls: 12400)
        4) bucket_destroy (% time: 9.10, self seconds: 0.01, calls: 17)
        5) string_knr_hash (% time: 4.55, self seconds: 0.01, calls: 67967)


gcc --coverage hash_table.c linked_list.c hash_table_tests.c -lcunit
./a.out
gcov -abcfu hash_table.c


# Code coverage

The following results were aquired with gcov. How to run gcov is described in the "Build and run" section in this document.


File 'hash_table.c'
Lines executed:94.92% of 197
Branches executed:96.61% of 59
Taken at least once:88.14% of 59
Calls executed:90.91% of 44

    Function 'find_previous_entry_for_key_ptr'
    Lines executed:75.00% of 8

    Function 'ioopm_lookup_key'
    Lines executed:83.33% of 6

    Function 'ioopm_hash_table_insert'
    Lines executed:94.44% of 18

    Function 'ioopm_hash_table_remove'
    Lines executed:66.67% of 18

    All other functions in hash_table.c not named above have 100% line coverage.


File 'linked_list.c'
Lines executed:100.00% of 149
Branches executed:100.00% of 45
Taken at least once:86.67% of 45
Calls executed:76.19% of 21

    All functions in linked_list.c has 100% line coverage.
