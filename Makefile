#To use gcc instead of g++
CC = gcc
#CFLAGS copied directly from libavl with -g replaced by -O3
CFLAGS = -O3 -W -Wall -ansi -pedantic
#LDFLAGS copied directly from libavl with -g replaced by -O3
LDFLAGS = -O3
#All implementations: O3 for optimization, o for output .exe with same name as file
tester:  
	$(CC) -DMEM_USAGE -O3 -o tester tester.c -I. -lm
1darray: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 1darray ./implementations/1darray.c -I. -lm
2dbst: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 2dbst ./implementations/2dbst.c -I. -lm
2dsll: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 2dsll ./implementations/2dsll.c -I. -lm
3darray: 
	$(CC) -DMEM_USAGE -DALONE -O3 -o 3darray ./implementations/3darray.c -I. -lm
avl_base:
	$(CC) -DMEM_USAGE -DALONE -O3 -o avl_base ./implementations/avl_base.c -I. -lm
rb_base:
	$(CC) -DMEM_USAGE -DALONE -O3 -o rb_base ./implementations/rb_base.c -I. -lm
bst:
	$(CC) -DMEM_USAGE -DALONE -O3 -o bst ./implementations/bst.c -I. -lm
hashtable:
	$(CC) -DMEM_USAGE -DALONE -O3 -o hashtable ./implementations/hashtable.c -I. -lm
.PHONY: clean
clean:
	rm -f tester 1darray bst 2dbst 1dsll 3darray avl_base rb_base hashtable