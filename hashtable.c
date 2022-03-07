/* 
  To compile:
  make hashtable

  For a list of command line options: ./hashtable

  Example usage: ./hashtable -i ./images/myimage.ppm
*/

// TODO: Fix/Check Memory Leaks

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

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

int main(int argc, char **argv)
{
    clock_t start, stop;
    double addtime, counttime;

    char in_file_name[256];
    RGB_Image *in_img;

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

    in_img = read_PPM(in_file_name);

    //Hash table histogram
    start = clock();

    int ih;
    uint hash;
    int index;
    uint red, green, blue;
    Bucket bucket;
    Hash_Table hash_table;

    int num_colors = 0;
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
            num_colors++;
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
    addtime = ((double) (stop - start)) / CLOCKS_PER_SEC;
    printf("\nTotal time to add and count all colors to histogram (hash table) = %g\n", addtime);
    printf("\nTotal number of colors in %s according to hash table count: %d\n", in_file_name, num_colors);

    free(bucket);
    free(hash_table);
    free(in_img->data);
    free(in_img);

    return EXIT_SUCCESS;
}
