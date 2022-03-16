/*
  To compile:
  make 1darray

  For a list of command line options: ./1darray

  Example usage: ./1darray -i ./images/myimage.ppm
*/

// TODO: Fix/Check Memory Leaks

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

// Maximum value for an any (r * 65536 + g * 256 + b) value
#define MAX_VAL_PACKED 16777216

// Iterate through a one dimensional array representing the color histogram.
// If at least a single color exists (> 0), than increment num_colors counter and return at end.
int count_colors_1d_histo(int histogram[MAX_VAL_PACKED])
{
    int num_colors = 0;
    for (int i = 0; i < MAX_VAL_PACKED; i++)
    {
        if (histogram[i] != 0)
        {
            num_colors++;
        }
    }
    return num_colors;
}

struct results* do1darray(RGB_Image *in_img)
{
    // 1-D array histogram
    // int *histogram;
    // histogram = malloc(sizeof *histogram * MAX_VAL_PACKED);
    // RGB_Pixel *pixel;
    // for (int i = 0; i < in_img->size; i++)
    // {
    //     pixel = &in_img->data[i];
    //     histogram[(pixel->red << 16) | (pixel->green << 8) | pixel->blue]++;
    // }
    // int num_cols_3darray = count_colors_1d_histo(histogram);
    // free(pixel);
    // free(histogram);

    struct results res;
    res.num_cols = 1;
    res.addtime = 1;
    res.counttime = 1;
    return res;
}

int main(int argc, char **argv)
{
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
        else
        {
            print_usage(argv[0]);
        }
    }

    in_img = read_PPM(in_file_name);
    struct results yourmom;
    yourmom = do1darray(&in_img);
    return 0;
}