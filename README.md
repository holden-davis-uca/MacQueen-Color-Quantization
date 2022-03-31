# About

*Base code forked form https://github.com/skythomp16/MacQueen-Color-Quantization*

Research on memory and time requirements of data structures used for color histograms in color quantization.

Holden Davis under Dr. E. Celebi of UCA Computer Science Department, Spring 2022.

# Current codebase:
## Folders:
- **.vscode**: My personal VS Code settings (irrelevant to research).
- **images**: A folder containing various .ppm images to test on.
    - baboon.ppm
    - fish.ppm
    - girl.ppm
    - goldhill.ppm
    - kodim05.ppm
    - kodim23.ppm
    - peppers.ppm
    - pills.ppm
    - frymire.ppm
    - kiss.ppm
## Implementations:
- **1darray.c**: Flattened 1-D array implementation of a color histogram.
- **2dbst.c**: 2-D array with Binary Search Tree element implementation of a color histogram.
- **2dsll.c**: 2-D array with Singly-Linked List element implementation of a color histogram.
- **3darray.c**: 3-D array implementation of a color histogram.
- **avl_base.c**: LibAVL AVL tree implementation of a color histogram.
- **rb_base.c**: LibAVL Red Black tree implementation of a color histogram.
- **avl_pp.c**: LibAVL AVL tree implementation of a color histogram with parent pointers.
- **rb_pp.c**: LibAVL Red Black tree implementation of a color histogram with parent pointers.
- **bst.c**: Binary Search Tree implementation of a color histogram.
- **hashtable.c**: Hash Table implementation of a color histogram.
- **aa_base.c**: AA tree implementation of a color histogram. - UNFINISHED, WIP
- **wavl_base.c**: WAVL tree implementation of a color histogram. - UNFINISHED, WIP
## Other:
- **avl.c**: LibAVL testing library.
- **avl.h**: LibAVL testing library.
- **rb.c**: LibAVL testing library.
- **rb.h**: LibAVL testing library.
- **pavl.c**: LibAVL testing library.
- **pavl.h**: LibAVL testing library.
- **prb.c**: LibAVL testing library.
- **prb.h**: LibAVL testing library.
- **AAT.c**: AA tree testing library.
- **AAT.h**: AA tree testing library.
- **alloc.h**: AA tree testing library.
- **bailout.h**: AA tree testing library.
- **common.h**: AA tree testing library.
- **wavltree.c**: WAVL tree testing library.
- **wavltree.h**: WAVL tree testing library.
- **wavltree_priv.h**: WAVL tree testing library.
- **research.code-workspace**: My personal VS Code workspace file (irrelevant to research).
- **rgb_structs.h**: Defines RGB structures for image analysis.
- **SKELETON.c**: A simple, empty, easily-filled framework for adding future implementations.
- **test.c**: LibAVL testing library.
- **test.h**: LibAVL testing library.
- **util.c**: Utility functions (such as image input).
- **mkm.c**: The original work from the above repository (used for image input/output functions).
- **tester.c**: a file combining all previous methods (used for testing timing and comparing methods against each other).

# Notes:

- Due to the common use case of this project by the authors, the project is currently configured to run solely via the compilation and execution of tester.c. In order to execute each standalone implementation as its own file, uncomment the two commented segments in the given file: the #include headers at the top and the main() function at the bottom.

- All implementations print a short message to the console indicating image, time performance and counted colors.

- The one exception to this is tester, which instead creates a file called data.txt, which contains a comparison table of all implementations executed on all files.

- All files except tester require at least a .ppm image as an argument.

- All files can accept a -r (number) argument, which executes the implementation a specific number of times and uses the average of those numbers as the result (all files execute a single run by default).

- Tester.c can accept a -d argument, which increases console verbosity for debug purposes

- Currently, all implementations are to be compiled with GNU Make.