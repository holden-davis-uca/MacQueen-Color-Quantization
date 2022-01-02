/* 
  To compile:
  g++ -O3 -o testsuite testsuite.c -lm
*/

#include <chrono>
#include <climits>
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>

using namespace std::chrono;

//Maximum value for an r, g, or b value
int const MAX_VAL = 256;

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

//Iterate through a three dimensional array representing the color histogram.
//If at least a single color exists (> 0), than increment num_colors counter and return at end.
int count_colors_3d_histo(int histogram[MAX_VAL][MAX_VAL][MAX_VAL]) {
    int num_colors = 0;
    for (int i = 0; i < MAX_VAL; i++) {
        for (int j = 0; j < MAX_VAL; j++) {
            for (int k = 0; k < MAX_VAL; k++) {
                if (histogram[i][j][k] != 0) {
                    num_colors++;
                }
            }
        }
    }
    return num_colors;
}

int num_cols_bst = 0;

void traverse_bst(const struct BST_Node* root)
{
    if (root != NULL)
    {
      num_cols_bst++;
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

int main(int argc, char **argv)
{
  FILE *results;
  results = fopen("results.txt", "w");
  fprintf(results, "%-25s%-25s%-25s%-25s%-25s%-25s%-25s","IMAGE", "TIME TO ADD (BST)", "TIME TO COUNT(BST)", "NUMBER OF COLORS (BST)", "TIME TO ADD (3-D)", "TIME TO COUNT (3-D)", "NUMER OF COLORS (3-D)");
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
    RGB_Pixel mean;
    RGB_Image *in_img;
    strcpy(in_file_name, pictures[z]);

    in_img = read_PPM(in_file_name, &mean);

    //3-D array histogram
    auto histogram = new int[MAX_VAL][MAX_VAL][MAX_VAL]{};
    auto start_3d_time = high_resolution_clock::now();
    for (int i = 0; i < in_img->size; i++){
        int redvalue = in_img->data[i].red;
        int greenvalue = in_img->data[i].green;
        int bluevalue = in_img->data[i].blue;
        histogram[redvalue][greenvalue][bluevalue]++;
    }

    auto stop_3d_time = high_resolution_clock::now();
    auto array3d_duration = duration_cast<microseconds>(stop_3d_time - start_3d_time);

    auto start_3d_count_time = high_resolution_clock::now();
    int num_cols_3darray = count_colors_3d_histo(histogram);
    auto stop_3d_count_time = high_resolution_clock::now();
    auto array3d_count_duration = duration_cast<microseconds>(stop_3d_count_time - start_3d_count_time);
    //BST histogram
    auto start_bst_time = high_resolution_clock::now();
    unsigned int red_value = in_img->data[0].red;
    unsigned int green_value = in_img->data[0].green;
    unsigned int blue_value = in_img->data[0].blue;
    int key = (red_value << 16) | (green_value << 8) | blue_value;
    struct BST_Node* root = insert_bst_node(NULL, key);
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
    traverse_bst(root);
    auto stop_bst_count_time = high_resolution_clock::now();
    auto bst_count_duration = duration_cast<microseconds>(stop_bst_count_time - start_bst_count_time);

    fprintf(results, "\n");
    fprintf(results, "%-25s%-25g%-25g%-25d%-25g%-25g%-25d",in_file_name, bst_duration.count() / 1e3, bst_count_duration.count() / 1e3, num_cols_bst, array3d_duration.count() / 1e3, array3d_count_duration.count() / 1e3, num_cols_3darray);
    
    num_cols_bst = 0;
    free(root->left);
    free(root->right);
    free(root);
    free(in_img->data);
    free(in_img);
    
  }
  fclose(results);
  return EXIT_SUCCESS;
}
