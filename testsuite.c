/* 
  To compile:
  g++ -O3 -o testsuite testsuite.c -lm

  For a list of command line options: ./testsuite
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

//Maximum value for an any (r * 65536 + g * 256 + b) value
#define MAX_VAL_PACKED 16777216

struct BST_Node
{
  int key;
  int count;
  struct BST_Node *left;
  struct BST_Node *right;
};

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

int traverse_2dsll(const struct SLL_Node *head)
{
  if (head == NULL)
  {
    return 0;
  }
  else
  {
    if (head->count == 0)
    {
      return (0 + traverse_2dsll(head->next));
    }
    else
      return (1 + traverse_2dsll(head->next));
  }
}

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

int traverse_bst(const struct BST_Node *root)
{
  if (root == NULL)
  {
    return 0;
  }
  else if (root->count == 0)
  {
    return 0 + traverse_bst(root->right) + traverse_bst(root->left);
  }
  else
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
  FILE *results;
  results = fopen("results.txt", "w");
  fprintf(results, "%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s", "IMAGE", "TIME TO ADD (2-D SLL)", "TIME TO COUNT (2-D SLL)", "NUMBER OF COLORS (2-D SLL)", "TIME TO ADD (2-D BST)", "TIME TO COUNT(2-D BST)", "NUMBER OF COLORS (2-D BST)", "TIME TO ADD (BST)", "TIME TO COUNT(BST)", "NUMBER OF COLORS (BST)", "TIME TO ADD (3-D)", "TIME TO COUNT (3-D)", "NUMBER OF COLORS (3-D)", "TIME TO ADD (1-D)", "TIME TO COUNT (1-D)", "NUMBER OF COLORS (1-D)");
  const char *pictures[8];
  pictures[0] = "./images/baboon.ppm";
  pictures[1] = "./images/fish.ppm";
  pictures[2] = "./images/girl.ppm";
  pictures[3] = "./images/goldhill.ppm";
  pictures[4] = "./images/kodim05.ppm";
  pictures[5] = "./images/kodim23.ppm";
  pictures[6] = "./images/peppers.ppm";
  pictures[7] = "./images/pills.ppm";
  for (int z = 0; z < 8; z++)
  {
    char in_file_name[256];
    RGB_Image *in_img;
    strcpy(in_file_name, pictures[z]);

    in_img = read_PPM(in_file_name);

    //1-D array histogram
    auto histogram1d = new int[MAX_VAL_PACKED]{};
    auto start_1d_time = high_resolution_clock::now();
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      histogram1d[(pixel->red << 16) | (pixel->green << 8) | pixel->blue]++;
    }
    auto stop_1d_time = high_resolution_clock::now();
    auto array1d_duration = duration_cast<microseconds>(stop_1d_time - start_1d_time);

    auto start_1d_count_time = high_resolution_clock::now();
    int num_cols_1darray = count_colors_1d_histo(histogram1d);
    auto stop_1d_count_time = high_resolution_clock::now();
    auto array1d_count_duration = duration_cast<microseconds>(stop_1d_count_time - start_1d_count_time);

    //3-D array histogram
    auto histogram3d = new int[MAX_VAL][MAX_VAL][MAX_VAL]{};
    auto start_3d_time = high_resolution_clock::now();
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      histogram3d[pixel->red][pixel->green][pixel->blue]++;
    }
    auto stop_3d_time = high_resolution_clock::now();
    auto array3d_duration = duration_cast<microseconds>(stop_3d_time - start_3d_time);

    auto start_3d_count_time = high_resolution_clock::now();
    int num_cols_3darray = count_colors_3d_histo(histogram3d);
    auto stop_3d_count_time = high_resolution_clock::now();
    auto array3d_count_duration = duration_cast<microseconds>(stop_3d_count_time - start_3d_count_time);

    //BST histogram
    auto start_bst_time = high_resolution_clock::now();
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
    auto stop_bst_time = high_resolution_clock::now();
    auto bst_duration = duration_cast<microseconds>(stop_bst_time - start_bst_time);
    auto start_bst_count_time = high_resolution_clock::now();
    int num_cols_bst = traverse_bst(root);
    auto stop_bst_count_time = high_resolution_clock::now();
    auto bst_count_duration = duration_cast<microseconds>(stop_bst_count_time - start_bst_count_time);

    //2-D BST
    auto start_2d_bst_time = high_resolution_clock::now();
    auto bst2darray = new struct BST_Node[MAX_VAL][MAX_VAL]
    {
    };
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      struct BST_Node *root = insert_bst_node(&bst2darray[pixel->red][pixel->green], pixel->blue);
    }
    auto stop_2d_bst_time = high_resolution_clock::now();
    auto bst_2d_duration = duration_cast<microseconds>(stop_2d_bst_time - start_2d_bst_time);
    auto start_2d_bst_count_time = high_resolution_clock::now();
    int num_cols_bst2d = count_colors_2dbst(bst2darray);
    auto stop_2d_bst_count_time = high_resolution_clock::now();
    auto bst_2d_count_duration = duration_cast<microseconds>(stop_2d_bst_count_time - start_2d_bst_count_time);

    //2-D SLL
    auto start_2d_sll_time = high_resolution_clock::now();
    auto sll2darray = new struct SLL_Node[MAX_VAL][MAX_VAL]
    {
    };

    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      struct SLL_Node *head = insert_sll_node(&sll2darray[pixel->red][pixel->green], pixel->blue);
    }
    auto stop_2d_sll_time = high_resolution_clock::now();
    auto sll_2d_duration = duration_cast<microseconds>(stop_2d_sll_time - start_2d_sll_time);
    auto start_2d_sll_count_time = high_resolution_clock::now();
    int num_cols_2dsll = count_colors_2dsll(sll2darray);
    auto stop_2d_sll_count_time = high_resolution_clock::now();
    auto sll_2d_count_duration = duration_cast<microseconds>(stop_2d_sll_count_time - start_2d_sll_count_time);

    fprintf(results, "\n");
    fprintf(results, "%-25s%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d", in_file_name, sll_2d_duration.count() / 1e3, sll_2d_count_duration.count() / 1e3, num_cols_2dsll, bst_2d_duration.count() / 1e3, bst_2d_count_duration.count() / 1e3, num_cols_bst2d, bst_duration.count() / 1e3, bst_count_duration.count() / 1e3, num_cols_bst, array3d_duration.count() / 1e3, array3d_count_duration.count() / 1e3, num_cols_3darray, array1d_duration.count() / 1e3, array1d_count_duration.count() / 1e3, num_cols_1darray);
    num_cols_bst2d = 0;
    num_cols_bst = 0;
    num_cols_2dsll = 0;
    free(root->left);
    free(root->right);
    free(root);
    free(in_img->data);
    free(in_img);
  }
  fclose(results);
  return EXIT_SUCCESS;
}
