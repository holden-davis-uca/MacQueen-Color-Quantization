/* 
  To compile:
  make avl_base

*/

// TODO: Fix/Check Memory Leaks

#include <chrono>
#include <climits>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"
#include "avl.c"

using namespace std::chrono;

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

  //AVL histogram
  auto start_bst_time = high_resolution_clock::now();
  for (int i = 0; i < in_img->size; i++)
  {
    uint red_value = in_img->data[i].red;
    uint green_value = in_img->data[i].green;
    uint blue_value = in_img->data[i].blue;
    uint key = (red_value << 16) | (green_value << 8) | blue_value;
  }
  auto stop_bst_time = high_resolution_clock::now();
  auto bst_duration = duration_cast<microseconds>(stop_bst_time - start_bst_time);
  printf("\nTotal time to add all colors to histogram (AVL) = %g\n", bst_duration.count() / 1e3);
  auto start_bst_count_time = high_resolution_clock::now();
  //int num_cols_avl = ???;
  auto stop_bst_count_time = high_resolution_clock::now();
  auto bst_count_duration = duration_cast<microseconds>(stop_bst_count_time - start_bst_count_time);
  printf("\nTotal time to count number of colors in histogram (AVL) = %g\n", bst_count_duration.count() / 1e3);
  std::cout << "\nTotal number of colors in " << in_file_name << " according to AVL count: " << num_cols_avl << "\n";
  
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
