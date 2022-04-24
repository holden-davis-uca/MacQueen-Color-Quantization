#To use gcc instead of g++
CC = gcc
#CFLAGS copied directly from libavl with -g replaced by -O3
CFLAGS = -O3 -W -Wall -ansi -pedantic
#LDFLAGS copied directly from libavl with -g replaced by -O3
LDFLAGS = -O3
#All implementations: O3 for optimization, o for output .exe with same name as file
tester:  
	$(CC) -O3 -o tester tester.c -I. 
1darray: 
	$(CC) -O3 -o 1darray 1darray.c -I.
2dbst: 
	$(CC) -O3 -o 2dbst 2dbst.c -I.
2dsll: 
	$(CC) -O3 -o 2dsll 2dsll.c -I.
3darray: 
	$(CC) -O3 -o 3darray 3darray.c -I.
avl_base:
	$(CC) -O3 -o avl_base avl_base.c -I.
wavl_base:
	$(CC) -O3 -o wavl_base wavl_base.c -I.
rb_base:
	$(CC) -O3 -o rb_base rb_base.c -I.
avl_pp:
	$(CC) -O3 -o avl_pp avl_pp.c -I.
rb_pp: 
	$(CC) -O3 -o rb_pp rb_pp.c -I.
bst: bst
	$(CC) -O3 -o bst bst.c -I.
hashtable:
	$(CC) -O3 -o hashtable hashtable.c -I.
hashtablebig: 
	$(CC) -O3 -o hashtablebig hashtablebig.c -I.
aa_base: 
	$(CC) -O3 -o aa_base aa_base.c -I.