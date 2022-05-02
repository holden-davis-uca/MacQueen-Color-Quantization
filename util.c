/*
The read_PPM function needed to read in the ppm images and extract relevant inforamtion (for us, colors)
*/

#include "rgb_structs.h"

typedef unsigned char uchar;
typedef unsigned long ulong;

//Maximum value for an r, g, or b value
int const MAX_VAL = 256;

#define HASH_SIZE 65537

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

static void print_usage(char *prog_name)
{
  fprintf(stderr, "Color Histogram Testing Program\n\n");
  fprintf(stderr, "Creates color histograms from input .ppm images. Prints a informative segment to the console describing time usage and image color count\n\n");
  fprintf(stderr, "Usage Instructions: %s required: -i <input image>, optionally: -r <number of runs>\n\n", prog_name);
  exit(EXIT_FAILURE);
}

static void print_usage_test(char *prog_name)
{
  fprintf(stderr, "Color Histogram Testing Program - Test Suite\n\n");
  fprintf(stderr, "Creates color histograms from input .ppm images. Prints a informative segment to the console describing time usage and image color count\n\n");
  fprintf(stderr, "Usage Instructions: %s required: N/A, optionally: -r <number of runs>, -d for debug output in console\n\n", prog_name);
  exit(EXIT_FAILURE);
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