/* 
  To compile:
  g++ -O3 -o 1darray 1darray.c -lm

  For a list of command line options: ./1darray
 */

#include <chrono>
#include <climits>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std::chrono;

//Maximum value for an any (r * 65536 + g * 256 + b) value
int const MAX_VAL_COMBINED = ((256 * 65536) + (256 * 256) + (256));

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


//Iterate through a one dimensional array representing the color histogram.
//If at least a single color exists (> 0), than increment num_colors counter and return at end.
int count_colors_1d_histo(int histogram[MAX_VAL_COMBINED]) {
    int num_colors = 0;
    for (int i = 0; i < MAX_VAL_COMBINED; i++)
    {
      if (histogram[i] != 0)
      {
        num_colors++;
      }
    }
    return num_colors;
}

RGB_Image *
read_PPM(const char *filename, RGB_Pixel *mean)
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

static void
print_usage(char *prog_name)
{
  fprintf(stderr, "Color Histogram with 3-Dimensional Array\n\n");
  fprintf(stderr, "Creates color histograms from input .ppm images with Binary Search Trees.\n\n");
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

  //1-D array histogram
  auto histogram = new int[MAX_VAL_COMBINED]{};
  auto start_3d_time = high_resolution_clock::now();
  for (int i = 0; i < in_img->size; i++){
    unsigned int red_value = in_img->data[i].red;
    unsigned int green_value = in_img->data[i].green;
    unsigned int blue_value = in_img->data[i].blue;
    int key = (red_value << 16) | (green_value << 8) | blue_value;
    histogram[key]++;
  }

  auto stop_3d_time = high_resolution_clock::now();
  auto array3d_duration = duration_cast<microseconds>(stop_3d_time - start_3d_time);
  printf("\nTotal time to add all colors to histogram (1-D array) = %g\n", array3d_duration.count() / 1e3);
  auto start_3d_count_time = high_resolution_clock::now();
  int num_cols_3darray = count_colors_1d_histo(histogram);
  auto stop_3d_count_time = high_resolution_clock::now();
  auto array3d_count_duration = duration_cast<microseconds>(stop_3d_count_time - start_3d_count_time);
  printf("\nTotal time to count number of colors in histogram (1-D array) = %g\n", array3d_count_duration.count() / 1e3);
  std::cout<<"\nTotal number of colors in " <<in_file_name << " according to 1d array count: " << num_cols_3darray << "\n";  

  free(in_img->data);
  free(in_img);

  return EXIT_SUCCESS;
}
