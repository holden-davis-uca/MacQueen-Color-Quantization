/*
RGB_Pixel and RGB_Image structures needed for image input and color count.
The remainder are implementation-specific
*/

//Unsigned typedefs
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

//RGB Pixel structure
typedef struct
{
  uint red, green, blue;
} RGB_Pixel;

//RGB Image structure
typedef struct
{
  int width, height;
  int size;
  RGB_Pixel *data;
} RGB_Image;

//Results struct to be returned by every implementation
typedef struct 
{
  int num_cols;
  double addtime;
  double counttime;
  int total_mem;
} results;

//BST Node (used in bst and 2d bst)
struct BST_Node
{
  int key;
  int count;
  struct BST_Node *left;
  struct BST_Node *right;
};

//SLL Node
struct SLL_Node
{
  int key;
  int count;
  struct SLL_Node *next;
};

//Hash table bucket
typedef struct Bucket_Entry *Bucket;

//Hash table bucket entry
struct Bucket_Entry
{
    uint red;
    uint green;
    uint blue;
    uint count;
    Bucket next;
};

//Hash table
typedef Bucket *Hash_Table;