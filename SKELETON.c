/*

To compile: make %FILENAME%

To run: ./%FILENAME% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

//Define all implementation specific things here

results doFILENAME(RGB_Image *in_img)
{
    clock_t start, stop;
    double addtime, counttime;
    results res;
    start = clock();
    //Do implementation adding here
    stop = clock();
    addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    //Do implementation counting here
    stop = clock();
    counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    res.num_cols = ???;
    res.addtime = addtime;
    res.counttime = counttime;
    //Do all free() calls here
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
        results res = doFILENAME(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
    }
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    printf("Average time to add colors over %d runs: %f", num_runs ,averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d",num_cols);
    return 0;
}