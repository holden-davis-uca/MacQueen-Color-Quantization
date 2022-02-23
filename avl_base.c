/* 
  To compile:
  make avl_base
*/

// TODO: Fix/Check Memory Leaks

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.c"
#include "avl.c"
#include "avl-test.c"

/* Comparison function for pointers to |int|s.
   |param| is not used. */
int
compare_ints (const void *pa, const void *pb, void *param)
{
  const int *a = pa;
  const int *b = pb;

  if (*a < *b)
    return -1;
  else if (*a > *b)
    return +1;
  else
    return 0;
}

int main(int argc, char **argv)
{
  clock_t start, stop;
  double addtime, counttime;

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
  start = clock();
  struct libavl_allocator allocator = avl_allocator_default;
  struct avl_table *tree = avl_create(compare_ints, NULL, &allocator);
  uint *insertions;
  insertions = malloc(sizeof *insertions * in_img->size);
  for (int i = 0; i < in_img->size; i++)
  {
    uint red_value = in_img->data[i].red;
    uint green_value = in_img->data[i].green;
    uint blue_value = in_img->data[i].blue;
    uint key = (red_value << 16) | (green_value << 8) | blue_value;
    insertions[i] = key;
    avl_probe(tree, &insertions[i]);
  }
  stop = clock();
  addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to add all colors to histogram (AVL) = %g\n", addtime);
  printf("\nTotal number of colors in %s according to AVL count: %zu", in_file_name, tree->avl_count);

  free(insertions);
  free(in_img->data);
  free(in_img);
  return EXIT_SUCCESS;
}
