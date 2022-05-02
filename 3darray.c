/*

To compile: make %3darray%

To run: ./%3darray% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#ifdef ALONE
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"
#endif

//Define all implementation specific things here
int count_colors_3d_histo(int histogram[MAX_VAL][MAX_VAL][MAX_VAL])
{
    int num_colors = 0;
    for (int i = 0; i < MAX_VAL; i++)
    {
        for (int j = 0; j < MAX_VAL; j++)
        {
            for (int k = 0; k < MAX_VAL; k++)
            {
                if (histogram[i][j][k] != 0)
                {
                    num_colors++;
                }
            }
        }
    }
    return num_colors;
}

results do3darray(RGB_Image *in_img)
{
    clock_t start, stop;
    results res;
    start = clock();
    RGB_Pixel *pixel;
    int(*histogram)[MAX_VAL][MAX_VAL] = malloc(sizeof(int[MAX_VAL][MAX_VAL][MAX_VAL]));
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        histogram[pixel->red][pixel->green][pixel->blue]++;
    }
    //Do memory counting here
    #ifdef MEM_USAGE
    res.total_mem = sizeof(int[MAX_VAL][MAX_VAL][MAX_VAL]);
    #endif
    stop = clock();
    res.addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    res.num_cols = count_colors_3d_histo(histogram);
    stop = clock();
    res.counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    free(histogram);
    return res;
}

#ifdef ALONE
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
    double totaladd = 0;
    double totalcount = 0;
    double averageadd = 0;
    double averagecount = 0;
    int num_cols = 0;
    int total_mem = 0;
    for (int i = 0; i < num_runs; i++)
    {
        results res = do3darray(in_img);
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