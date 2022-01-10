/* 
  To compile:
  g++ -O3 -o 3darray 3darray.c -lm

  For a list of command line options: ./3darray
*/

#include <chrono>
#include <climits>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

using namespace std::chrono;

//Maximum value for an r, g, or b value
int const MAX_VAL = 256;

//Iterate through a three dimensional array representing the color histogram.
//If at least a single color exists (> 0), than increment num_colors counter and return at end.
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

  //3-D array histogram
  RGB_Pixel *pixel;

  auto start_3d_time = high_resolution_clock::now();
  auto histogram = new int[MAX_VAL][MAX_VAL][MAX_VAL]{};
  for (int i = 0; i < in_img->size; i++)
  {
  pixel = &in_img->data[i];
  histogram[pixel->red][pixel->green][pixel->blue]++;
  }
  auto stop_3d_time = high_resolution_clock::now();
  auto array3d_duration = duration_cast<microseconds>(stop_3d_time - start_3d_time);
  printf("\nTotal time to add all colors to histogram (3-D array) = %g\n", array3d_duration.count() / 1e3);
  auto start_3d_count_time = high_resolution_clock::now();
  int num_cols_3darray = count_colors_3d_histo(histogram);
  auto stop_3d_count_time = high_resolution_clock::now();
  auto array3d_count_duration = duration_cast<microseconds>(stop_3d_count_time - start_3d_count_time);
  printf("\nTotal time to count number of colors in histogram (3-D array) = %g\n", array3d_count_duration.count() / 1e3);
  std::cout << "\nTotal number of colors in " << in_file_name << " according to 3d array count: " << num_cols_3darray << "\n";

  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
