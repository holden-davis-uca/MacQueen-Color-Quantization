/* 
  To compile:
  g++ -O3 -o bst bst.c -lm

  For a list of command line options: ./bst
*/

// TODO: Fix/Check Memory Leaks
//NOTE: ONLY WORKS WITH G++/GCC, DOES NOT WORK WITH MAKE

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

struct BST_Node
{
  int key;
  int count;
  struct BST_Node *left;
  struct BST_Node *right;
};

struct BST_Node *alloc_bst_node(const int new_key)
{
  struct BST_Node *here = (struct BST_Node *)malloc(sizeof(struct BST_Node));
  here->key = new_key;
  here->count = 1;
  here->left = NULL;
  here->right = NULL;
  return here;
}

struct BST_Node *insert_bst_node(struct BST_Node *node, const int new_key)
{
  if (node == NULL)
  {
    return alloc_bst_node(new_key);
  }
  if (new_key == node->key)
  {
    node->count++;
  }
  else if (new_key > node->key)
  {
    node->right = insert_bst_node(node->right, new_key);
  }
  else
  {
    node->left = insert_bst_node(node->left, new_key);
  }
  return node;
}

int traverse_bst(const struct BST_Node *root)
{
  if (root == NULL)
  {
    return 0;
  }
  else if (root->count == 0)
  {
    return traverse_bst(root->right) + traverse_bst(root->left);
  }
  return 1 + traverse_bst(root->right) + traverse_bst(root->left);
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

  //BST histogram
  
  start = clock();
  unsigned int red_value = in_img->data[0].red;
  unsigned int green_value = in_img->data[0].green;
  unsigned int blue_value = in_img->data[0].blue;
  int key = (red_value << 16) | (green_value << 8) | blue_value;
  struct BST_Node *root = insert_bst_node(NULL, key);
  for (int i = 1; i < in_img->size; i++)
  {
    red_value = in_img->data[i].red;
    green_value = in_img->data[i].green;
    blue_value = in_img->data[i].blue;
    key = (red_value << 16) | (green_value << 8) | blue_value;
    insert_bst_node(root, key);
  }
 
  stop = clock();
  addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to add all colors to histogram (BST) = %g\n", addtime);
  start = clock();
  int num_cols_bst = traverse_bst(root);
  stop = clock();
  counttime = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("\nTotal time to count number of colors in histogram (BST) = %g\n", counttime);
  printf("\nTotal number of colors in %s according to bst count: %d\n", in_file_name, num_cols_bst);

  free(root);
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
