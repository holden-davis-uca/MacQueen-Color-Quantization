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

//Need a node definition; basically just a key comprised from rgb value and a count index to track duplicates
struct AVL_Node
{
  uint key;
  int count;
};

//Need a method to compare nodes
int compare_nodes (const void *pa, const void *pb)
{
  const struct AVL_Node *a = pa;
  const struct AVL_Node *b = pb;

  if (a->key < b->key)
    return -1;
  else if (a->key > b->key)
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
  struct avl_table *tree = avl_create(compare_nodes,NULL,&avl_allocator_default);

  start = clock();
  for (int i = 0; i < in_img->size; i++)
  {
    uint red_value = in_img->data[i].red;
    uint green_value = in_img->data[i].green;
    uint blue_value = in_img->data[i].blue;
    uint key = (red_value << 16) | (green_value << 8) | blue_value;
    //Make avl node with key
    //Query tree for node key; if not found, avl_insert, otherwise access and increment count by 1
    // if (avl_find(tree, node))
    // {
    //   //Node = avl_find(tree, key);
    //   //Node->count++;
    // }
    // else
    // {
    //   //avl_insert(tree, node)
    // }
  }
  stop = clock();
  addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to add all colors to histogram (AVL) = %g\n", addtime);
  start = clock();
  int num_cols_avl = 0;
  stop = clock();
  counttime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to count number of colors in histogram (AVL) = %g\n", counttime);
  printf("\nTotal number of colors in %s according to AVL count: %g", in_file_name, num_cols_avl);
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
