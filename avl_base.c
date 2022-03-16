/*

To compile: make %avl_base%

To run: ./%avl_base% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"
#include "avl.c"

results doavl_base(RGB_Image *in_img)
{
    clock_t start, stop;
    double addtime, counttime;
    results res;
    start = clock();
    struct libavl_allocator allocator = avl_allocator_default;
    struct avl_table *tree = avl_create(compare_ints, NULL, &allocator);
    uint *insertions = malloc(sizeof *insertions * in_img->size);
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        uint key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
        insertions[i] = key;
        avl_probe(tree, &insertions[i]);
    }
    stop = clock();
    addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    int avl_cols = tree->avl_count;
    stop = clock();
    counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    res.num_cols = avl_cols;
    res.addtime = addtime;
    res.counttime = counttime;
    free(insertions);
    free(tree);
    return res;
}

int main(int argc, char **argv)
{
    int num_runs = 1;
    char in_file_name[256];
    RGB_Image *in_img;
    if (argc == 1)
    {
        print_usage(argv[0]);
    }
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
        else
        {
            print_usage(argv[0]);
        }
    }
    in_img = read_PPM(in_file_name);
    double totaladd, totalcount, averageadd, averagecount;
    int num_cols;
    for (int i = 0; i < num_runs; i++)
    {
        results res = doavl_base(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
    }
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    printf("Average time to add colors over %d runs: %f", num_runs, averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d", num_cols);
    return 0;
}