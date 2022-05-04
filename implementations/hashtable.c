/*

To compile: make %hashtable%

To run: ./%hashtable% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#ifdef ALONE
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "./lib/util.c"
#endif

results dohashtable(RGB_Image *in_img)
{
    clock_t start, stop;
    results res;
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
    res.addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    //Do implementation counting here
    res.num_cols = num_colors;
    stop = clock();
    res.counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    //Do memory counting here
    #ifdef MEM_USAGE
    res.total_mem = (sizeof(struct Bucket_Entry) * num_colors) + sizeof(HASH_SIZE * sizeof(Bucket));
    #endif
    dealloc_ht(hash_table);
    return res;
}

#ifdef ALONE
int main(int argc, char **argv)
{
    int num_runs = 1;
    char in_file_name[256];
    RGB_Image *in_img;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-i"))
        {
            strcpy(in_file_name, argv[++i]);
        }
        else if (!strcmp(argv[i], "-r"))
        {
            num_runs = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-i") && strcmp(argv[i], "-r"))
        {
            print_usage(argv[0]);
        }
    }
    in_img = read_PPM(in_file_name);
    double totaladd = 0;
    double totalcount = 0;
    double averageadd = 0;
    double averagecount = 0;
    int num_cols = 0;
    int total_mem = 0;
    for (int i = 0; i < num_runs; i++)
    {
        results res = dohashtable(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
        #ifdef MEM_USAGE
        total_mem = res.total_mem;
        #endif
    }
    free(in_img->data);
    free(in_img);
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    printf("\n<!---Hash Table---!>\n\n");
    #ifdef MEM_USAGE
    printf("Bytes of memory used: %d\n", total_mem);
    printf("Bytes per color: %lu\n", (total_mem / num_cols));
    #endif
    printf("Average time to add colors over %d runs: %f", num_runs ,averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d",num_cols);
    printf("\n\n");
    return 0;
}
#endif