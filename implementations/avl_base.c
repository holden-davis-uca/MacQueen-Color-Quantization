/*

To compile: make %avl_base%

To run: ./%avl_base% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#ifdef ALONE
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "./lib/util.c"
#include "./lib/avl.c"
#endif

results doavl_base(RGB_Image *in_img)
{
    clock_t start, stop;
    results res;
    start = clock();
    struct libavl_allocator allocator = avl_allocator_default;
    struct avl_table *tree = avl_create(compare_ints, NULL, &allocator);
    uint *insertions = malloc(sizeof(uint) * in_img->size);
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        uint key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
        insertions[i] = key;
        avl_probe(tree, &insertions[i]);
    }
    #ifdef MEM_USAGE
    res.total_mem = sizeof(uint) * in_img->size;
    #endif
    stop = clock();
    res.addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    res.num_cols = tree->avl_count;
    stop = clock();
    res.counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    avl_destroy(tree, NULL);
    free(insertions);
    return res;
}
#ifdef ALONE
int main(int argc, char **argv)
{
    int num_runs = 1;
    char in_file_name[256];
    RGB_Image *in_img;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-i"))
        {
            strcpy(in_file_name, argv[++i]);
        }
        else if (!strcmp(argv[i], "-r"))
        {
            num_runs = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-i") && strcmp(argv[i], "-r"))
        {
            print_usage(argv[0]);
        }
    }
    in_img = read_PPM(in_file_name);
    double totaladd = 0;
    double totalcount = 0;
    double averageadd = 0;
    double averagecount = 0;
    int num_cols = 0;
    int total_mem = 0;
    for (int i = 0; i < num_runs; i++)
    {
        results res = doavl_base(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
        #ifdef MEM_USAGE
        total_mem = res.total_mem;
        #endif
    }
    free(in_img->data);
    free(in_img);
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    printf("\n<!---AVL Tree---!>\n\n");
    #ifdef MEM_USAGE
    printf("Bytes of memory used: %d\n", total_mem);
    printf("Bytes per color: %lu\n", (total_mem / num_cols));
    #endif
    printf("Average time to add colors over %d runs: %f", num_runs ,averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d",num_cols);
    printf("\n\n");
    return 0;
}
#endif 