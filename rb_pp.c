/*

To compile: make %rb_pp%

To run: ./%rb_pp% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

// #include <stdlib.h>
// #include <time.h>
// #include <math.h>
// #include <string.h>
// #include <stdio.h>
// #include "util.c"
// #include "prb.c"

//Define all implementation specific things here

//Trigger executed by make command -DMEM_USAGE to count bytes used by the program
// #define MEM_USAGE

results dorb_pp(RGB_Image *in_img)
{
    clock_t start, stop;
    results res;
    start = clock();
    struct libavl_allocator allocator = prb_allocator_default;
    struct prb_table *tree = prb_create(compare_ints, NULL, &allocator);
    uint *insertions = malloc(sizeof(uint) * in_img->size);
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        uint key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
        insertions[i] = key;
        prb_probe(tree, &insertions[i]);
    }
    #ifdef MEM_USAGE
    res.total_mem = sizeof(uint) * in_img->size;
    #endif
    stop = clock();
    res.addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    res.num_cols = tree->prb_count;
    stop = clock();
    res.counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    free(insertions);
    free(tree);
    return res;
}

// int main(int argc, char **argv)
// {
//     int num_runs = 1;
//     char in_file_name[256];
//     RGB_Image *in_img;
//     if (argc == 1)
//     {
//         print_usage(argv[0]);
//     }
//     for (int i = 1; i < argc; i++)
//     {
//         if (!strcmp(argv[i], "-i"))
//         {
//             strcpy(in_file_name, argv[++i]);
//         }
//         else if (!strcmp(argv[i], "-r"))
//         {
//             num_runs = atoi(argv[++i]);
//         }
//         else
//         {
//             print_usage(argv[0]);
//         }
//     }
//     in_img = read_PPM(in_file_name);
//     double totaladd, totalcount, averageadd, averagecount;
//     int num_cols, total_mem;
//     for (int i = 0; i < num_runs; i++)
//     {
//         results res = dorb_pp(in_img);
//         totaladd += res.addtime;
//         totalcount += res.counttime;
//         num_cols = res.num_cols;
//         #ifdef MEM_USAGE
//         total_mem += res.total_mem;
//         #endif
//     }
//     averageadd = totaladd / num_runs;
//     averagecount = totalcount / num_runs;
//     #ifdef MEM_USAGE
//     printf("%d bytes of memory used\n", total_mem);
//     #endif
//     printf("Average time to add colors over %d runs: %f", num_runs ,averageadd);
//     printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
//     printf("\nNumber of unique colors: %d",num_cols);
//     return 0;
// }