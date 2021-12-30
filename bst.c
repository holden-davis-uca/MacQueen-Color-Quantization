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

struct BSTNode
{
    int rgbkey;
    int count;
    struct BSTNode *left;
    struct BSTNode *right;
};

struct BSTNode* newNode(int newkey)
{
    struct BSTNode* here = (struct BSTNode*)malloc(sizeof(struct BSTNode));
    here->rgbkey = newkey;
    here->count = 1;
    here->left = NULL;
    here->right = NULL;
    return here;
}

struct BSTNode* insert(struct BSTNode* node, int newrgbkey)
{
    if (node == NULL)
    {
        return newNode(newrgbkey);
    }
    if (newrgbkey == node->rgbkey)
    {
        node->count++;
    }
    if (newrgbkey > node->rgbkey)
    {
        node->right = insert(node->right, newrgbkey);
    }
    if (newrgbkey < node->rgbkey)
    {
        node->left = insert(node->left, newrgbkey);
    }
    return node;

}

void traverse(struct BSTNode* rootnode)
{
    if (rootnode != NULL)
    {
        traverse(rootnode->left);
        std::cout<<"RGB Key = " << rootnode->rgbkey << "; Color count = " << rootnode->count << std::endl;
        traverse(rootnode->right);
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
  auto start3 = high_resolution_clock::now();
  int redvalue = in_img->data[0].red;
  int greenvalue = in_img->data[0].green;
  int bluevalue = in_img->data[0].blue;
  int key = (redvalue * 65536) + (greenvalue * 256) + bluevalue;
  struct BSTNode* root = insert(root, key);
  for (int i = 1; i < in_img->size; i++)
  {
    int redvalue = in_img->data[i].red;
    int greenvalue = in_img->data[i].green;
    int bluevalue = in_img->data[i].blue;
    int newkey = (redvalue * 65536) + (greenvalue * 256) + bluevalue;
    insert(root, newkey);
  }
  auto stop3 = high_resolution_clock::now();
  auto duration3 = duration_cast<microseconds>(stop3 - start3);
  printf("\nTotal time to add all colors to histogram (BST) = %g\n", duration3.count() / 1e3);
  //traverse(root);
  
  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
