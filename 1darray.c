/* 
  To compile:
  g++ -O3 -o 1darray 1darray.c -lm

  For a list of command line options: ./1darray
*/

// TODO: Fix/Check Memory Leaks

#include <chrono>
#include <climits>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

using namespace std::chrono;

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
  char in_file_name[256];
  RGB_Image *in_img, *out_img;

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
  auto start_3d_time = high_resolution_clock::now();
  auto histogram = new int[MAX_VAL_PACKED]{};
  RGB_Pixel *pixel;
  for (int i = 0; i < in_img->size; i++)
  {
    pixel = &in_img->data[i];
    histogram[(pixel->red << 16) | (pixel->green << 8) | pixel->blue]++;
  }
  auto stop_3d_time = high_resolution_clock::now();
  auto array3d_duration = duration_cast<microseconds>(stop_3d_time - start_3d_time);
  printf("\nTotal time to add all colors to histogram (1-D array) = %g\n", array3d_duration.count() / 1e3);
  auto start_3d_count_time = high_resolution_clock::now();
  int num_cols_3darray = count_colors_1d_histo(histogram);
  auto stop_3d_count_time = high_resolution_clock::now();
  auto array3d_count_duration = duration_cast<microseconds>(stop_3d_count_time - start_3d_count_time);
  printf("\nTotal time to count number of colors in histogram (1-D array) = %g\n", array3d_count_duration.count() / 1e3);
  std::cout << "\nTotal number of colors in " << in_file_name << " according to 1d array count: " << num_cols_3darray << "\n";

  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
