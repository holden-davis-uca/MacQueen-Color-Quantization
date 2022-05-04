#gcc as the preferred compiler
CC = gcc
#-DMEM_USAGE for memory usage information
#-DALONE to compile each implementation as a standalone program
#-O3 for compiler optimization
#-I for the include directory
#-lm for linker accuracy
#-o for the output file name
COREFLAGS = -DMEM_USAGE -DALONE -O3 -I. -lm -o
#Same as above but without -DALONE in order to run all implementations inside of tester
TESTERFLAGS = -DMEM_USAGE -O3 -I. -lm -o
#Leave cleaning directive empty, if windows use del, if linux use rm
NUKE = 
ifeq ($(OS),Windows_NT)
	NUKE += del *.exe *.txt
else
	NUKE += rm -f 1darray 2dbst 2dsll bst 3darray hashtable avl_base rb_base *.txt
endif

#By default, this makefile's made command compiles and executes all implementations then the tester in sequence at 1 run each then cleans everything
all:
	make 1darray
	./1darray -i ./images/1931.ppm
	make 2dbst
	./2dbst -i ./images/1931.ppm
	make 2dsll
	./2dsll -i ./images/1931.ppm
	make 3darray
	./3darray -i ./images/1931.ppm
	make avl_base
	./avl_base -i ./images/1931.ppm
	make rb_base
	./rb_base -i ./images/1931.ppm
	make bst
	./bst -i ./images/1931.ppm
	make hashtable
	./hashtable -i ./images/1931.ppm
	make tester
	./tester -d
	make clean
tester:
	make clean
	$(CC) $(TESTERFLAGS) tester tester.c
1darray: 
	make clean
	$(CC) $(COREFLAGS) 1darray ./implementations/1darray.c
2dbst: 
	make clean
	$(CC) $(COREFLAGS) 2dbst ./implementations/2dbst.c
2dsll: 
	make clean
	$(CC) $(COREFLAGS) 2dsll ./implementations/2dsll.c
3darray: 
	make clean
	$(CC) $(COREFLAGS) 3darray ./implementations/3darray.c
avl_base:
	make clean
	$(CC) $(COREFLAGS) avl_base ./implementations/avl_base.c
rb_base:
	make clean
	$(CC) $(COREFLAGS) rb_base ./implementations/rb_base.c
bst:
	make clean
	$(CC) $(COREFLAGS) bst ./implementations/bst.c
hashtable:
	make clean
	$(CC) $(COREFLAGS) hashtable ./implementations/hashtable.c
.PHONY: clean
clean:
	$(NUKE)