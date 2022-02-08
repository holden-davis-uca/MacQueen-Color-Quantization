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
  printf("\n\nSTARTING\n\n");
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
  //THIS BELOW EXAMPLE WORKS
  // printf("Making default avl allocator...");
  // struct libavl_allocator allocator = avl_allocator_default;
  // printf("Finished!\n");
  // printf("Making avl...");
  // struct avl_table *tree = avl_create(compare_ints, NULL, &allocator);
  // if (tree == NULL)
  // {
  //   printf("Something went wrong\n");
  // }
  // printf("Finished!\n");
  // int inserts[10] = {9, 0, 2, 8, 3, 7, 6, 5, 4, 1};
  // for (int i = 0; i < 10; i++)
  // {
  //   printf("Inserting %i...", inserts[i]);
  //   void **p = avl_probe(tree, &inserts[i]);
  //   printf("Success!\n");
  //   print_whole_tree(tree, "Current Tree: ");
  //   printf("Current tree count: %zu\n", tree->avl_count);
  // }

  printf("Making default avl allocator...");
  struct libavl_allocator allocator = avl_allocator_default;
  printf("Finished!\n");
  printf("Making avl...");
  struct avl_table *tree = avl_create(compare_ints, NULL, &allocator);
  if (tree == NULL)
  {
    printf("Something went wrong\n");
  }
  printf("Finished!\n");
  uint *insertions;
  insertions = malloc(sizeof *insertions * 9999999);
  for (int i = 0; i < in_img->size; i++)
  {
    uint red_value = in_img->data[i].red;
    uint green_value = in_img->data[i].green;
    uint blue_value = in_img->data[i].blue;
    uint key = (red_value << 16) | (green_value << 8) | blue_value;
    insertions[i] = key;
    //printf("Inserting %i...", insertions[i]);
    void **p = avl_probe(tree, &insertions[i]);
    if (p == NULL)
    {
      printf("Problem\n");
    }
    //printf("Success!\n");
    //print_whole_tree(tree, "Current Tree: ");
    //printf("Current tree count: %zu\n", tree->avl_count);
  }
  //print_whole_tree(tree, "Current Tree: ");
  printf("Current tree count: %zu\n", tree->avl_count);



  // start = clock();
  // for (int i = 0; i < in_img->size; i++)
  // {
  //   uint red_value = in_img->data[i].red;
  //   uint green_value = in_img->data[i].green;
  //   uint blue_value = in_img->data[i].blue;
  //   uint key = (red_value << 16) | (green_value << 8) | blue_value;
  // }
  // printf("Key to insert: %u\n", key);
  //   void **p = avl_probe(tree, &key);
  //   if (p == NULL){
  //     printf("Shits fucked");
  //   }
  //   print_whole_tree(tree, "Tree after inserting key: ");
  // print_tree_structure(tree->avl_root, 0);
  // printf("\nNumber of elements: %zu\n", tree->avl_count);
  // stop = clock();
  // addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  // printf("\nTotal time to add all colors to histogram (AVL) = %g\n", addtime);
  // start = clock();
  // int num_cols_avl = 0;
  // stop = clock();
  // counttime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  // printf("\nTotal time to count number of colors in histogram (AVL) = %g\n", counttime);
  // printf("\nTotal number of colors in %s according to AVL count: %g", in_file_name, num_cols_avl);

  free(in_img->data);
  free(in_img);
  printf("\n\nSTOPPING\n\n");
  return EXIT_SUCCESS;
}
