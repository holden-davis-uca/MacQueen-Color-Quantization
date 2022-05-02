/*
RGB_Pixel and RGB_Image structures needed for image input and color count.
The remainder are implementation-specific
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

typedef struct 
{
  int num_cols;
  double addtime;
  double counttime;
  int total_mem;
} results;

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