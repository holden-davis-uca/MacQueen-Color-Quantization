# About

*Base code forked form https://github.com/skythomp16/MacQueen-Color-Quantization*

Research on memory and time requirements of data structures used for color histograms in color quantization.

Holden Davis under Dr. E. Celebi of UCA Computer Science Department, Spring 2022.

# Todo:
- Fix HTB counting method
- Re-evaluate BST memory usage

# Current codebase:
## Folders:
- **.vscode**: My personal VS Code settings (irrelevant to research).
- **headers**: C header files required for various implementations.
    - **avl.h**: LibAVL header file (required for AVL)
    - **rb.h**: LibAVL header file (required for RB)
    - **rb_structs.h**: A header I wrote to contain the various structures used by different implementations to store color data.
    - **test.h**: LibAVL header (required for both AVL and RB)
- **images**: A folder containing various .ppm images to test on.
    - 1931.ppm
    - bags.ppm
    - balloons.ppm
    - blur.ppm
    - candy.ppm
    - chairs.ppm
    - chart.ppm
    - dissolve.ppm
    - fruit.ppm
    - frymire.ppm
    - hotair.ppm
    - kiss.ppm
    - market.ppm
    - palette.ppm
    - pencils.ppm
    - pigments.ppm
    - wood.ppm
- **implementations**: All of the individual color histogram data structure implementations.
    - **1darray.c**: Flattened 1-D array implementation of a color histogram.
    - **2dbst.c**: 2-D array with Binary Search Tree element implementation of a color histogram.
    - **2dsll.c**: 2-D array with Singly-Linked List element implementation of a color histogram.
    - **3darray.c**: 3-D array implementation of a color histogram.
    - **avl_base.c**: LibAVL AVL tree implementation of a color histogram.
    - **rb_base.c**: LibAVL Red Black tree implementation of a color histogram.
    - **bst.c**: Binary Search Tree implementation of a color histogram.
    - **hashtable.c**: Hash Table implementation of a color histogram.
- **lib**: Various C libraries required for different implementations.
    - **avl.c**: LibAVL library (required for AVL)
    - **rb.c**: LibAVL library (required for RB)
    - **SKELETON.c**: Skeleton file I wrote to quickly and easily build future implementations
    - **test.c**: LibAVL library (required for both AVL and RB)
    - **util.c**: A library containing various functions and definitions that are shared between implementations
## Files:
- **Makefile**: GNU Make instructions to compile every implementation individually or together (by using the tester) with gcc
- **README.md**: This!
- **research.code-workspace**: My personal VS Code workspace file (irrelevant to research).
- **tester.c**: A singular testing suite file that encompasses all implementations. 
    - Can be run with a debug switch for verbose console ouput and a run number switch to average a larger number of consecutive runs. 
    - Runs all implementations on all images and generates a file called **data.txt** containing numerical results for comparison, including time to add colors (denoted ADD), time to count colors (denoted CNT), number of colors (denoted CLR), and memory usage in bytes per unique color (denoted MEM).

# Notes:

- By default, all files and the implementation also collect information on memory used in addition to time (individual files output the number of bytes per unique color and tester adds an additional column to the output file for each structure's memory usage) - this can be turned off by removing the -DMEM_USAGE switch in the Makefile for the relevant implementation.

- The 1D and 3D array implementations do not report memory in the output file as their bytes per color ratio is the same given that they allocate the same memory regardless of image size.

- All implementations print a short message to the console indicating image, time performance and counted colors.

- The one exception to this is tester, which instead creates a file called data.txt, which contains a comparison table of all implementations executed on all files.

- All files except tester require at least a .ppm image as an argument.

- All files can accept a -r (number) argument, which executes the implementation a specific number of times and uses the average of those numbers as the result (all files execute a single run by default).

- Tester.c can accept a -d argument, which increases console verbosity for debug purposes

- Currently, all implementations are to be compiled with GNU Make.

[AVL tree source](https://adtinfo.org/)
