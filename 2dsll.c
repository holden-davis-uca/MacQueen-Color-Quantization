/* 
  To compile:
  make 2dsll

  For a list of command line options: ./2dsll

  Example usage: ./2dsll -i ./images/myimage.ppm
*/

// TODO: Fix/Check Memory Leaks

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

//Maximum value for an r, g, or b value
int const MAX_VAL = 256;

struct SLL_Node
{
  int key;
  int count;
  struct SLL_Node *next;
};

struct SLL_Node *alloc_sll_node(const int new_key)
{
  struct SLL_Node *here = (struct SLL_Node *)malloc(sizeof(struct SLL_Node));
  here->key = new_key;
  here->count = 1;
  here->next = NULL;
  return here;
}

struct SLL_Node *insert_sll_node(struct SLL_Node *node, const int new_key)
{
  if (node == NULL)
  {
    return alloc_sll_node(new_key);
  }
  if (new_key == node->key)
  {
    node->count++;
  }
  else
  {
    node->next = insert_sll_node(node->next, new_key);
  }
  return node;
}

//TODO: Fix counting logic
int traverse_2dsll(const struct SLL_Node *head)
{
 if (head == NULL)
  {
    return 0;
  }
 else if (head->count == 0)
  {
   return traverse_2dsll(head->next);
  }
   

 return 1 + traverse_2dsll(head->next);
}

int count_colors_2dsll(struct SLL_Node sll2darray[MAX_VAL][MAX_VAL])
{
  int num_cols_2dsll = 0;
  for (int i = 0; i < MAX_VAL; i++)
  {
    for (int j = 0; j < MAX_VAL; j++)
    {
      num_cols_2dsll += traverse_2dsll(&sll2darray[i][j]);
    }
  }
  return num_cols_2dsll;
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

  //2-D sll histogram
  start = clock();
  struct SLL_Node (*sll2darray)[MAX_VAL] = malloc(sizeof(struct SLL_Node[MAX_VAL][MAX_VAL]));

  RGB_Pixel *pixel;
  for (int i = 0; i < in_img->size; i++)
  {
    pixel = &in_img->data[i];
    struct SLL_Node *head = insert_sll_node(&sll2darray[pixel->red][pixel->green], pixel->blue);
  }
  stop = clock();
  addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to add all colors to histogram (2-D SLL) = %g\n", addtime);
  start = clock();
  int num_cols_2dsll = count_colors_2dsll(sll2darray);
  stop = clock();
  counttime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to count number of colors in histogram (2-D SLL) = %g\n", counttime);
  printf("\nTotal number of colors in %s according to 2d SLL count: %d\n", in_file_name, num_cols_2dsll);

  free(pixel);
  free(sll2darray);
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
