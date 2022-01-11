*Baseline code functionality forked form https://github.com/skythomp16/MacQueen-Color-Quantization*

# Current codebase:

- **images**, a folder containing various .ppm images to test on.
    - baboon.ppm
    - fish.ppm
    - girl.ppm
    - goldhill.ppm
    - kodim05.ppm
    - kodim23.ppm
    - peppers.ppm
    - pills.ppm
- **mkm.c**, the original work from the above repository (used for image input/output functions).
- **1darray.c**, the implementation of a color histogram using a flattened 1-D array.
- **3darray.c**, the implementation of a color histogram using a 3-D array.
- **2dsll.c**, the implementation of a color histogram using a 2-D array whose elements are singly linked lists.
- **2dbst.c**, the implementation of a color histogram using a 2-D array whose elements are binary search trees.
- **bst.c**, the implementation of a color histogram using a binary search tree.
- **testsuite.c**, a file combining all previous methods (used for testing timing and comparing methods against each other).

All files except testsuite require at least a .ppm image as an argument.

REMINDER TO FIX MEMORY LEAKS