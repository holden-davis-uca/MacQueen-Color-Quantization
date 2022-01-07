/*
RGB_Pixel and RGB_Image structures needed for image input and color count
*/

typedef unsigned int uint;

typedef struct
{
  uint red, green, blue;
} RGB_Pixel;

typedef struct
{
  int width, height;
  int size;
  RGB_Pixel *data;
} RGB_Image;