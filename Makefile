#To use gcc instead of g++
CC = gcc
#CFLAGS copied directly from libavl with -g replaced by -O3
CFLAGS = -O3 -W -Wall -ansi -pedantic
#LDFLAGS copied directly from libavl with -g replaced by -O3
LDFLAGS = -O3
#All implementations: O3 for optimization, o for output .exe with same name as file
tester:  
	$(CC) -DMEM_USAGE -O3 -o tester tester.c -I. 
1darray: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 1darray 1darray.c -I.
2dbst: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 2dbst 2dbst.c -I.
2dsll: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 2dsll 2dsll.c -I.
3darray: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 3darray 3darray.c -I.
avl_base:
	$(CC) -DMEM_USAGE -DALONE -O3 -o avl_base avl_base.c -I.
rb_base:
	$(CC) -DMEM_USAGE -DALONE -O3 -o rb_base rb_base.c -I.
avl_pp:
	$(CC) -DMEM_USAGE -DALONE -O3 -o avl_pp avl_pp.c -I.
rb_pp: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o rb_pp rb_pp.c -I.
bst: bst
	$(CC) -DMEM_USAGE -DALONE -O3 -o bst bst.c -I.
hashtable:
	$(CC) -DMEM_USAGE -DALONE -O3 -o hashtable hashtable.c -I.
hashtablebig: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o hashtablebig hashtablebig.c -I.
.PHONY: clean
clean:
	rm -f tester 1darray 2dbst 1dsll 3darray avl_base rb_base avl_pp rb_pp hashtable hashtablebig