/* 
  To compile:
  g++ -O3 -o bst bst.c -lm

  For a list of command line options: ./bst
*/

#include <chrono>
#include <climits>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std::chrono;

typedef unsigned char uchar;
typedef unsigned long ulong;

typedef struct
{
  double red, green, blue;
} RGB_Pixel;

typedef struct
{
  int size;
  RGB_Pixel center;
} RGB_Cluster;

typedef struct
{
  int width, height;
  int size;
  RGB_Pixel *data;
} RGB_Image;

struct BST_Node
{
    int key;
    int count;
    struct BST_Node *left;
    struct BST_Node *right;
};

struct BST_Node* alloc_bst_node(const int new_key)
{
    struct BST_Node* here = (struct BST_Node*)malloc(sizeof(struct BST_Node));
    here->key = new_key;
    here->count = 1;
    here->left = NULL;
    here->right = NULL;
    return here;
}

struct BST_Node* insert_bst_node(struct BST_Node* node, const int new_key)
{
    if (node == NULL)
    {
        return alloc_bst_node(new_key);
    }
    if (new_key == node->key)
    {
        node->count++;
    }
    else if ( new_key > node->key )
    {
    node->right = insert_bst_node( node->right, new_key );
    }
    else
    {
    node->left = insert_bst_node ( node->left, new_key );
    }
    return node;
}

int num_cols_bst = 0;

void traverse_bst(const struct BST_Node* root)
{
    if (root != NULL)
    {
      if (root->count != 0)
      {
        num_cols_bst++;
      }
      traverse_bst(root ->left);
      //std::cout<<"RGB Key = " << root->key << "; Color count = " << root->count << std::endl;
      traverse_bst(root->right);
    }
}

RGB_Image * read_PPM(const char *filename, RGB_Pixel *mean)
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
  mean->red = mean->green = mean->blue = 0.0;
  while (fread(&byte, 1, 1, fp) && i < img->size)
  {
    pixel = &img->data[i];
    mean->red += (pixel->red = byte);
    fread(&byte, 1, 1, fp);
    mean->green += (pixel->green = byte);
    fread(&byte, 1, 1, fp);
    mean->blue += (pixel->blue = byte);
    i++;
  }

  mean->red /= img->size;
  mean->green /= img->size;
  mean->blue /= img->size;

  fclose(fp);

  return img;
}

static void print_usage(char *prog_name)
{
  fprintf(stderr, "Color Histogram with Binary Search Tree\n\n");
  fprintf(stderr, "Creates color histograms from input .ppm images with 3-D arrays.\n\n");
  fprintf(stderr, "Usage: %s -i <input image>\n\n", prog_name);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    char in_file_name[256];
  RGB_Pixel mean;
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

  in_img = read_PPM(in_file_name, &mean);

  //BST histogram
  auto start_bst_time = high_resolution_clock::now();
  unsigned int red_value = in_img->data[0].red;
  unsigned int green_value = in_img->data[0].green;
  unsigned int blue_value = in_img->data[0].blue;
  int key = (red_value << 16) | (green_value << 8) | blue_value;
  struct BST_Node* root = insert_bst_node(root, key);
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
  printf("\nTotal time to add all colors to histogram (BST) = %g\n", bst_duration.count() / 1e3);
  auto start_bst_count_time = high_resolution_clock::now();
  traverse_bst(root);
  auto stop_bst_count_time = high_resolution_clock::now();
  auto bst_count_duration = duration_cast<microseconds>(stop_bst_count_time - start_bst_count_time);
  printf("\nTotal time to count number of colors in histogram (BST) = %g\n", bst_count_duration.count() / 1e3);
  std::cout<<"\nTotal number of colors in " <<in_file_name << " according to bst count: " << num_cols_bst << "\n";
  
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
