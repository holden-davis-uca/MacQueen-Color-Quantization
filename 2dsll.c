/*

To compile: make %2dsll%

To run: ./%2dsll% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"

struct SLL_Node *alloc_sll_node(const int new_key)
{
    struct SLL_Node *here = (struct SLL_Node *)malloc(sizeof(struct SLL_Node));
    here->key = new_key;
    here->count = 1;
    here->next = NULL;
    return here;
}

struct SLL_Node *insert_sll_node(struct SLL_Node *node, const int new_key)
{
    if (node == NULL)
    {
        return alloc_sll_node(new_key);
    }
    if (new_key == node->key)
    {
        node->count++;
    }
    else
    {
        node->next = insert_sll_node(node->next, new_key);
    }
    return node;
}

// TODO: Fix counting logic
int traverse_2dsll(const struct SLL_Node *head)
{
    if (head == NULL)
    {
        return 0;
    }
    else if (head->count == 0)
    {
        return traverse_2dsll(head->next);
    }

    return 1 + traverse_2dsll(head->next);
}

int count_colors_2dsll(struct SLL_Node sll2darray[MAX_VAL][MAX_VAL])
{
    int num_cols_2dsll = 0;
    for (int i = 0; i < MAX_VAL; i++)
    {
        for (int j = 0; j < MAX_VAL; j++)
        {
            num_cols_2dsll += traverse_2dsll(&sll2darray[i][j]);
        }
    }
    return num_cols_2dsll;
}

results do2dsll(RGB_Image *in_img)
{
    clock_t start, stop;
    double addtime, counttime;
    results res;
    start = clock();
    struct SLL_Node(*sll2darray)[MAX_VAL] = malloc(sizeof(struct SLL_Node[MAX_VAL][MAX_VAL]));
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        struct SLL_Node *head = insert_sll_node(&sll2darray[pixel->red][pixel->green], pixel->blue);
    }
    stop = clock();
    addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    int num_cols_2dsll = count_colors_2dsll(sll2darray);
    stop = clock();
    counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    res.num_cols = num_cols_2dsll;
    res.addtime = addtime;
    res.counttime = counttime;
    free(sll2darray);
    return res;
}

int main(int argc, char **argv)
{
    int num_runs = 1;
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
        else if (!strcmp(argv[i], "-r"))
        {
            num_runs = atoi(argv[++i]);
        }
        else
        {
            print_usage(argv[0]);
        }
    }
    in_img = read_PPM(in_file_name);
    double totaladd, totalcount, averageadd, averagecount;
    int num_cols;
    for (int i = 0; i < num_runs; i++)
    {
        results res = do2dsll(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
    }
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    printf("Average time to add colors over %d runs: %f", num_runs, averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d", num_cols);
    return 0;
}