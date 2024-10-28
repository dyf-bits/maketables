gcc -c src/mylib.c -o src/mylib.o
gcc src/make_scim_tables.c src/mylib.o -o bin/maketables
rm src/mylib.o
