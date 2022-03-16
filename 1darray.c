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



//Maximum value for an any (r * 65536 + g * 256 + b) value
#define MAX_VAL_PACKED 16777216

//Iterate through a one dimensional array representing the color histogram.
//If at least a single color exists (> 0), than increment num_colors counter and return at end.
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

int main(int argc, char **argv)
{
  clock_t start, stop;
  double addtime, counttime;

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

  //1-D array histogram
  start = clock();
  int *histogram;
  histogram = malloc(sizeof *histogram * MAX_VAL_PACKED);
  RGB_Pixel *pixel;
  for (int i = 0; i < in_img->size; i++)
  {
    pixel = &in_img->data[i];
    histogram[(pixel->red << 16) | (pixel->green << 8) | pixel->blue]++;
  }
  stop = clock();
  addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to add all colors to histogram (1-D array) = %g\n", addtime);
  start = clock();
  int num_cols_3darray = count_colors_1d_histo(histogram);
  stop = clock();
  counttime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to count number of colors in histogram (1-D array) = %g\n", counttime);
  printf("\nTotal number of colors in %s according to 1d array count: %d\n", in_file_name, num_cols_3darray);

  free(pixel);
  free(histogram);
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
