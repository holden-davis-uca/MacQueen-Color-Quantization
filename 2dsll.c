/* 
  To compile:
  g++ -O3 -o 2dsll 2dsll.c -lm

  For a list of command line options: ./2dsll
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

  //2-D sll histogram
  auto start_2d_sll_time = high_resolution_clock::now();
  auto sll2darray = new struct SLL_Node[MAX_VAL][MAX_VAL]{};
  for (int i = 0; i < in_img->size; i++)
  {
    int red_value = in_img->data[i].red;
    int green_value = in_img->data[i].green;
    int blue_value = in_img->data[i].blue;
    struct SLL_Node *head = insert_sll_node(&sll2darray[red_value][green_value], blue_value);
  }
  auto stop_2d_sll_time = high_resolution_clock::now();
  auto sll_2d_duration = duration_cast<microseconds>(stop_2d_sll_time - start_2d_sll_time);
  printf("\nTotal time to add all colors to histogram (2-D SLL) = %g\n", sll_2d_duration.count() / 1e3);
  auto start_2d_sll_count_time = high_resolution_clock::now();
  int num_cols_2dsll = 0;
  for (int i = 0; i < MAX_VAL; i++)
  {
    for (int j = 0; j < MAX_VAL; j++)
    {
      num_cols_2dsll += traverse_2dsll(&sll2darray[i][j]);
    }
  }
  auto stop_2d_sll_count_time = high_resolution_clock::now();
  auto sll_2d_count_duration = duration_cast<microseconds>(stop_2d_sll_count_time - start_2d_sll_count_time);
  printf("\nTotal time to count number of colors in histogram (2-D SLL) = %g\n", sll_2d_count_duration.count() / 1e3);
  std::cout << "\nTotal number of colors in " << in_file_name << " according to 2-D SLL count: " << num_cols_2dsll << "\n";

  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
