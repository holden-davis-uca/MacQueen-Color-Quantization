/* 
  To compile:
  make tester

  Example usage: ./tester
*/

// TODO: Fix/Check Memory Leaks

#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.c"
#include "avl.c"
#include "avl-test.c"

//Maximum value for an r, g, or b value
int const MAX_VAL = 256;

//Maximum value for an any (r * 65536 + g * 256 + b) value
#define MAX_VAL_PACKED 16777216

#define HASH_SIZE 20023

/* TODO: Do we need bitmasking in the HASH function? */

#define HASH(R, G, B) ((((long)(R)*33023 +  \
                         (long)(G)*30013 +  \
                         (long)(B)*27011) & \
                        0x7fffffff) %       \
                       HASH_SIZE)

typedef struct Bucket_Entry *Bucket;

struct Bucket_Entry
{
    uint red;
    uint green;
    uint blue;
    uint count;
    Bucket next;
};

typedef Bucket *Hash_Table;

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
  double 
  addtime1d, counttime1d, 
  addtime2dbst, counttime2dbst, 
  addtime2dsll, counttime2dsll, 
  addtime3d, counttime3d, 
  addtimebst, counttimebst,
  addtimeht,
  addtimeavl;

  FILE *results;
  results = fopen("results.txt", "w");

  fprintf(results, 
  "%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s", 
  "IMAGE",
  "ADD (AVL)", "COUNT (AVL)","COLORS (AVL)",
  "ADD (Hash Table)","COUNT (Hash Table)","COLORS (Hash Table)",
  "ADD (2-D SLL)", "COUNT (2-D SLL)", "COLORS (2-D SLL)",
  "ADD (2-D BST)", "COUNT(2-D BST)", "COLORS (2-D BST)",
  "ADD (BST)", "COUNT(BST)", "COLORS (BST)",
  "ADD (3-D)", "COUNT (3-D)", "COLORS (3-D)",
  "ADD (1-D)", "COUNT (1-D)", "COLORS (1-D)");
  
  const char *pictures[10];  
  pictures[0] = "./images/baboon.ppm";
  pictures[1] = "./images/fish.ppm";
  pictures[2] = "./images/girl.ppm";
  pictures[3] = "./images/goldhill.ppm";
  pictures[4] = "./images/kodim05.ppm";
  pictures[5] = "./images/kodim23.ppm";
  pictures[6] = "./images/peppers.ppm";
  pictures[7] = "./images/pills.ppm";
  pictures[8] = "./images/frymire.ppm";
  pictures[9] = "./images/kiss.ppm";
  for (int z = 0; z < 10; z++)
  {
    char in_file_name[256];
    RGB_Image *in_img;
    strcpy(in_file_name, pictures[z]);

    in_img = read_PPM(in_file_name);

    //1-D array histogram
    printf("Processing %s with 1-D array histogram\n", pictures[z]);
    start = clock();
    int *histogram1d;
    histogram1d = malloc(sizeof *histogram1d * MAX_VAL_PACKED);    start = clock();
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      histogram1d[(pixel->red << 16) | (pixel->green << 8) | pixel->blue]++;
    }
    stop = clock();
    addtime1d = ((double) (stop - start)) / CLOCKS_PER_SEC;

    start = clock();
    int num_cols_1darray = count_colors_1d_histo(histogram1d);
    stop = clock();
    counttime1d = ((double) (stop - start)) / CLOCKS_PER_SEC;

    //3-D array histogram
    printf("Processing %s with 3-D array histogram\n", pictures[z]);
    int (*histogram3d)[MAX_VAL][MAX_VAL] = malloc(sizeof(int[MAX_VAL][MAX_VAL][MAX_VAL]));
    start = clock();
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      histogram3d[pixel->red][pixel->green][pixel->blue]++;
    }
    stop = clock();
    addtime3d = ((double) (stop - start)) / CLOCKS_PER_SEC;

    start = clock();
    int num_cols_3darray = count_colors_3d_histo(histogram3d);
    stop = clock();
    counttime3d = ((double) (stop - start)) / CLOCKS_PER_SEC;

    //BST histogram
    printf("Processing %s with BST histogram\n", pictures[z]);
    start = clock();
    pixel = &in_img->data[0];
    int key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
    struct BST_Node *root = insert_bst_node(NULL, key);
    for (int i = 1; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
      insert_bst_node(root, key);
    }
    stop = clock();
    addtimebst = ((double) (stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    int num_cols_bst = traverse_bst(root);
    stop = clock();
    counttimebst = ((double) (stop - start)) / CLOCKS_PER_SEC;

    //2-D BST
    printf("Processing %s with 2-D BST histogram\n", pictures[z]);
    start = clock();
    struct BST_Node (*bst2darray)[MAX_VAL] = malloc(sizeof(struct BST_Node[MAX_VAL][MAX_VAL]));
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      struct BST_Node *root = insert_bst_node(&bst2darray[pixel->red][pixel->green], pixel->blue);
    }
    stop = clock();
    addtime2dbst = ((double) (stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    int num_cols_bst2d = count_colors_2dbst(bst2darray);
    stop = clock();
    counttime2dbst = ((double) (stop - start)) / CLOCKS_PER_SEC;

    //2-D SLL
    printf("Processing %s with 2-D SLL histogram\n", pictures[z]);
    start = clock();
    struct SLL_Node (*sll2darray)[MAX_VAL] = malloc(sizeof(struct SLL_Node[MAX_VAL][MAX_VAL]));

    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      struct SLL_Node *head = insert_sll_node(&sll2darray[pixel->red][pixel->green], pixel->blue);
    }
    stop = clock();
    addtime2dsll = ((double) (stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    int num_cols_2dsll = count_colors_2dsll(sll2darray);
    stop = clock();
    counttime2dsll = ((double) (stop - start)) / CLOCKS_PER_SEC;

    //Hash table
    printf("Processing %s with hash table histogram\n", pictures[z]);
    start = clock();

    int ih;
    uint hash;
    int index;
    uint red, green, blue;
    Bucket bucket;
    Hash_Table hash_table;

    int num_cols_hashtable = 0;
    hash_table = (Hash_Table)malloc(HASH_SIZE * sizeof(Bucket));

    for (ih = 0; ih < HASH_SIZE; ih++)
    {
        hash_table[ih] = NULL;
    }

    for (int i = 0; i < in_img->size; i++)
    {

        red = in_img->data[i].red;
        green = in_img->data[i].green;
        blue = in_img->data[i].blue;

        /* Determine the bucket */
        hash = HASH(red, green, blue);

        /* Search for the color in the bucket chain */
        for (bucket = hash_table[hash]; bucket != NULL; bucket = bucket->next)
        {

            if (bucket->red == red && bucket->green == green && bucket->blue == blue)
            {
                /* This color exists in the hash table */
                break;
            }
        }
        if (bucket != NULL)
        {
            /* This color exists in the hash table */
            bucket->count++;
        }
        else
        {
            num_cols_hashtable++;
            /* Create a new bucket entry for this color */
            bucket = (Bucket)malloc(sizeof(struct Bucket_Entry));

            bucket->red = red;
            bucket->green = green;
            bucket->blue = blue;
            bucket->count = 1;
            bucket->next = hash_table[hash];
            hash_table[hash] = bucket;
        }
    }

    stop = clock();
    addtimeht = ((double) (stop - start)) / CLOCKS_PER_SEC;

    //AVL histogram
    printf("Processing %s with AVL histogram\n", pictures[z]);
    struct libavl_allocator allocator = avl_allocator_default;
    struct avl_table *tree = avl_create(compare_ints, NULL, &allocator);
    uint *insertions;
    insertions = malloc(sizeof *insertions * in_img->size);
    for (int i = 0; i < in_img->size; i++)
    {
      pixel = &in_img->data[i];
      uint key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
      insertions[i] = key;
      avl_probe(tree, &insertions[i]);
    }
    stop = clock();
    addtimeavl = ((double) (stop - start)) / CLOCKS_PER_SEC;
    /*
    Filename (String)

    AVL add (double)
    AVL colors (%zu)

    Hash table add (double)
    Hash table count (double) - 0 
    Hash table colors (int)

    2dsll add (double)
    2dsll count (double)
    2dsll colors (int)

    2dbst add (double)
    2dbst count (double)
    2dbst colors (int)

    bst add (double)
    bst count (double)
    bst colors (int)

    3d add (double)
    3d count (double)
    3d colors (int)

    1d add (double)
    1d count (double)
    1d colors (int)

    */
    fprintf(results, "\n");
    fprintf(results, 
    "%-25s%-25g%-25d%-25zu%-25g%-25d%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d", 
    in_file_name, 
    addtimeavl, 0, tree->avl_count,
    addtimeht, 0, num_cols_hashtable, 
    addtime2dsll, counttime2dsll, num_cols_2dsll, 
    addtime2dbst, counttime2dbst, num_cols_bst2d,
    addtimebst, counttimebst, num_cols_bst,
    addtime1d, counttime1d, num_cols_1darray,
    addtime3d, counttime3d, num_cols_3darray);
    
    num_cols_bst2d = 0;
    num_cols_bst = 0;
    num_cols_2dsll = 0;
    num_cols_hashtable = 0;
    num_cols_1darray = 0;

    free(tree);
    free(hash_table);
    free(root->left);
    free(root->right);
    free(root);
    free(in_img->data);
    free(in_img);
  }
  fclose(results);
  printf("\nCheck results.txt for stats!\n\n");
  return EXIT_SUCCESS;
}
