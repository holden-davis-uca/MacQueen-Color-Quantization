/* 
  To compile:
  g++ -O3 -o 2dbst 2dbst.c -lm

  For a list of command line options: ./2dbst
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

//Maximum value for an r, g, or b value
int const MAX_VAL = 256;

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

// TODO: Fix counting logic
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

int count_colors_2dbst(struct BST_Node bst2darray[MAX_VAL][MAX_VAL])
{
  int num_cols_2dbst = 0;
  for (int i = 0; i < MAX_VAL; i++)
  {
    for (int j = 0; j < MAX_VAL; j++)
    {
      num_cols_2dbst += traverse_bst(&bst2darray[i][j]);
    }
  }
  return num_cols_2dbst;
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

  //2-D BST histogram
  auto start_2d_bst_time = high_resolution_clock::now();
  auto bst2darray = new struct BST_Node[MAX_VAL][MAX_VAL]{};
  RGB_Pixel *pixel;
  for (int i = 0; i < in_img->size; i++)
  {
  pixel = &in_img->data[i];
  struct BST_Node *root = insert_bst_node(&bst2darray[pixel->red][pixel->green], pixel->blue);
  }
  auto stop_2d_bst_time = high_resolution_clock::now();
  auto bst_2d_duration = duration_cast<microseconds>(stop_2d_bst_time - start_2d_bst_time);
  printf("\nTotal time to add all colors to histogram (BST) = %g\n", bst_2d_duration.count() / 1e3);
  auto start_2d_bst_count_time = high_resolution_clock::now();
  int num_cols_2dbst = count_colors_2dbst(bst2darray);
  auto stop_2d_bst_count_time = high_resolution_clock::now();
  auto bst_2d_count_duration = duration_cast<microseconds>(stop_2d_bst_count_time - start_2d_bst_count_time);
  printf("\nTotal time to count number of colors in histogram (2-D BST) = %g\n", bst_2d_count_duration.count() / 1e3);
  std::cout << "\nTotal number of colors in " << in_file_name << " according to bst count: " << num_cols_2dbst << "\n";
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
