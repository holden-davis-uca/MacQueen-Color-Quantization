#To use gcc instead of g++
CC = gcc
#CFLAGS copied directly from libavl
CFLAGS = -g -W -Wall -ansi -pedantic
#LDFLAGS copied directly from libavl
LDFLAGS = -g
#All implementations: O3 for optimization, o for output .exe with same name as file
tester: tester 
	$(CC) -O3 -o tester tester.c -I.
1darray: 1darray
	$(CC) -O3 -o 1darray 1darray.c -I.
2dbst: 2dbst
	$(CC) -O3 -o 2dbst 2dbst.c -I.
2dsll: 2dsll
	$(CC) -O3 -o 2dsll 2dsll.c -I.
3darray: 3darray
	$(CC) -O3 -o 3darray 3darray.c -I.
avl_base: avl_base 
	$(CC) -O3 -o avl_base avl_base.c -I.
bst: bst
	$(CC) -O3 -o bst bst.c -I.
hashtable: hashtable
	$(CC) -O3 -o hashtable hashtable.c -I.