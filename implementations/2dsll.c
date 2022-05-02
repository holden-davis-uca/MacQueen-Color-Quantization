/*

To compile: make %2dsll%

To run: ./%2dsll% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#ifdef ALONE
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "./lib/util.c"
#endif

results do2dsll(RGB_Image *in_img)
{
    clock_t start, stop;
    results res;
    start = clock();
    struct SLL_Node(*sll2darray)[MAX_VAL] = malloc(sizeof(struct SLL_Node[MAX_VAL][MAX_VAL]));
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        struct SLL_Node *head = insert_sll_node(&sll2darray[pixel->red][pixel->green], pixel->blue);
    }
    stop = clock();
    res.addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    res.num_cols = count_colors_2dsll(sll2darray);
    stop = clock();
    res.counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    #ifdef MEM_USAGE
    res.total_mem = sizeof(struct SLL_Node[MAX_VAL][MAX_VAL]) + (sizeof(struct SLL_Node) * res.num_cols);
    #endif
    free(sll2darray);
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
        results res = do2dsll(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
        #ifdef MEM_USAGE
        total_mem = res.total_mem;
        #endif
    }
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    #ifdef MEM_USAGE
    printf("%d bytes of memory used\n", total_mem);
    #endif
    printf("Average time to add colors over %d runs: %f", num_runs ,averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d",num_cols);
    return 0;
}
#endif