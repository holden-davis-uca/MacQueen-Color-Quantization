/*
Various utility functions
*/

#include "./headers/rgb_structs.h"

//Maximum value for an r, g, or b value
#define MAX_VAL 256

// Maximum value for an any (r * 65536 + g * 256 + b) value
#define MAX_VAL_PACKED 16777216

//Hash size for hash table
#define HASH_SIZE 65537

//Hash function
#define HASH(R, G, B) ((((long)(R)*33023 +  \
                         (long)(G)*30013 +  \
                         (long)(B)*27011)) %       \
                       HASH_SIZE)

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

// Iterate through a one dimensional array representing the color histogram.
// If at least a single color exists (> 0), than increment num_colors counter and return at end.
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

//BST traversal function; counts number of elements in a BST
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

//Deallocate bst from the root node
void dealloc_bst(struct BST_Node* root)
{
  if (root != NULL)
  {
    dealloc_bst(root->right);
    dealloc_bst(root->left);
    free(root);
  }
  else return;
}

//Allocates bst node
struct BST_Node *alloc_bst_node(const int new_key)
{
    struct BST_Node *here = (struct BST_Node *)malloc(sizeof(struct BST_Node));
    here->key = new_key;
    here->count = 1;
    here->left = NULL;
    here->right = NULL;
    return here;
}

//Inserts bst node
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

//Count colors in 3d array
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

//Allocate SLL node
struct SLL_Node *alloc_sll_node(const int new_key)
{
    struct SLL_Node *here = (struct SLL_Node *)malloc(sizeof(struct SLL_Node));
    here->key = new_key;
    here->count = 1;
    here->next = NULL;
    return here;
}

//Inserts SLL node
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

//Traverse 2dsll
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

//Count colors in 2dsll
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

//Count colors in 2dbst
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

//Read in PPM image and return as RGB_Image struct
RGB_Image * read_PPM(const char *filename)
{
  uchar byte;
  char buff[16];
  int c, max_rgb_val, i = 0;
  FILE *fp;
  RGB_Pixel *pixel;
  RGB_Image *img;

  fp = fopen(filename, "rb");
  if (!fp)
  {
    fprintf(stderr, "Unable to open file '%s'!\n", filename);
    exit(EXIT_FAILURE);
  }

  /* read image format */
  if (!fgets(buff, sizeof(buff), fp))
  {
    perror(filename);
    exit(EXIT_FAILURE);
  }

  /*check the image format to make sure that it is binary */
  if (buff[0] != 'P' || buff[1] != '6')
  {
    fprintf(stderr, "Invalid image format (must be 'P6')!\n");
    exit(EXIT_FAILURE);
  }

  img = (RGB_Image *)malloc(sizeof(RGB_Image));
  if (!img)
  {
    fprintf(stderr, "Unable to allocate memory!\n");
    exit(EXIT_FAILURE);
  }

  /* skip comments */
  c = getc(fp);
  while (c == '#')
  {
    while (getc(fp) != '\n')
      ;
    c = getc(fp);
  }

  ungetc(c, fp);

  /* read image dimensions */
  if (fscanf(fp, "%u %u", &img->width, &img->height) != 2)
  {
    fprintf(stderr, "Invalid image dimensions ('%s')!\n", filename);
    exit(EXIT_FAILURE);
  }

  /* read maximum component value */
  if (fscanf(fp, "%d", &max_rgb_val) != 1)
  {
    fprintf(stderr, "Invalid maximum R, G, B value ('%s')!\n", filename);
    exit(EXIT_FAILURE);
  }

  /* validate maximum component value */
  if (max_rgb_val != 255)
  {
    fprintf(stderr, "'%s' is not a 24-bit image!\n", filename);
    exit(EXIT_FAILURE);
  }

  while (fgetc(fp) != '\n')
    ;

  /* allocate memory for pixel data */
  img->size = img->height * img->width;
  img->data = (RGB_Pixel *)malloc(img->size * sizeof(RGB_Pixel));

  if (!img)
  {
    fprintf(stderr, "Unable to allocate memory!\n");
    exit(EXIT_FAILURE);
  }

  /* Read in pixels using buffer and calculate center of mass */
  while (fread(&byte, 1, 1, fp) && i < img->size)
  {
    pixel = &img->data[i];
    pixel->red = byte;
    fread(&byte, 1, 1, fp);
    pixel->green = byte;
    fread(&byte, 1, 1, fp);
    pixel->blue = byte;
    i++;
  }

  fclose(fp);

  return img;
}

//Usage info for standalone implementations
static void print_usage(char *prog_name)
{
  fprintf(stderr, "Color Histogram Testing Program\n\n");
  fprintf(stderr, "Creates color histograms from input .ppm images. Prints a informative segment to the console describing time usage and image color count\n\n");
  fprintf(stderr, "Usage Instructions: %s required: -i <input image>, optionally: -r <number of runs>\n\n", prog_name);
  exit(EXIT_FAILURE);
}

//Usage info for tester
static void print_usage_test(char *prog_name)
{
  fprintf(stderr, "Color Histogram Testing Program - Test Suite\n\n");
  fprintf(stderr, "Creates color histograms from input .ppm images. Prints a informative segment to the console describing time usage and image color count\n\n");
  fprintf(stderr, "Usage Instructions: %s required: N/A, optionally: -r <number of runs>, -d for debug output in console\n\n", prog_name);
  exit(EXIT_FAILURE);
}
