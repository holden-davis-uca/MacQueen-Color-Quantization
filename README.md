The idea behind this branch is to test if a larger refactor would be beneficial to prevent excess overlap of code

Where a single main suite includes every other file and calls do1darray() or whatnot and is returned a struct or something that contains three values. Those three values would be the number of colors, time to add and time to count

Single file:

// has all includes for .h c libraries and util.c and rgb_structs.c

has two base methods:
    one is do/methodname/ like do1darray(takes argument of RGB_Image) - this does all of the actual stuff and returns the triplet struct containing the relevant values - probably does its own timing as well
    other is main() - this is what occurs when called solo, takes argument of filename (via argc/argv as before) (and run number?) and just calls the above