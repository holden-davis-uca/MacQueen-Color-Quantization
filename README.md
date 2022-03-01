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
## Implementations:
- **1darray.c**: Flattened 1-D array implementation of a color histogram.
- **2dbst.c**: 2-D array with Binary Search Tree element implementation of a color histogram.
- **2dsll.c**: 2-D array with Singly-Linked List element implementation of a color histogram.
- **3darray.c**: 3-D array implementation of a color histogram.
- **avl_base.c**: LibAVL implementation of a color histogram.
- **bst.c**: Binary Search Tree implementation of a color histogram.
- **hashtable.c**: Hash Table implementation of a color histogram.
## Other:
- **avl-test.c**: LibAVL testing library.
- **avl.c**: LibAVL testing library.
- **avl.h**: LibAVL testing library.
- **research.code-workspace**: My personal VS Code workspace file (irrelevant to research).
- **rgb_structs.h**: Defines RGB structures for image analysis.
- **test.c**: LibAVL testing library.
- **test.h**: LibAVL testing library.
- **util.c**: Utility functions (such as image input).
- **mkm.c**: The original work from the above repository (used for image input/output functions).
- **tester.c**: a file combining all previous methods (used for testing timing and comparing methods against each other).

**All files except tester require at least a .ppm image as an argument.**

**Currently, all implementations are to be compiled with GNU Make.**