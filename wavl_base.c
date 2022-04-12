/*

To compile: make %wavl_base%

To run: ./%wavl_base% -i %PPM_IMAGE_PATH% -r %NUMBER_OF_RUNS%

It can be run with just the image argument and the number of runs will default to 1

*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "util.c"
#include "wavltree.c"

//Define all implementation specific things here
struct test_node {
    ptrdiff_t id; /**< For testing purposes, the ID of the node is our key */
    struct wavl_tree_node node;
};

#define TEST_NODE(_x) WAVL_CONTAINER_OF((_x), struct test_node, node)

static
wavl_result_t _test_node_compare_func(ptrdiff_t lhs,
                                      ptrdiff_t rhs,
                                      int *pdir)
{
    *pdir = (int)(lhs - rhs);
    return WAVL_ERROR_OK;
}

/**
 * Comparison function (for testing), to compare node-to-node
 */
static
wavl_result_t _test_node_to_node_compare_func(struct wavl_tree_node *lhs,
                                              struct wavl_tree_node *rhs,
                                              int *pdir)
{
    return _test_node_compare_func(TEST_NODE(lhs)->id, TEST_NODE(rhs)->id, pdir);
}

/**
 * Comparison function (for testing), to compare key-to-node
 */
static
wavl_result_t _test_node_to_value_compare_func(void *key_lhs,
                                               struct wavl_tree_node *rhs,
                                               int *pdir)
{
    return _test_node_compare_func((ptrdiff_t)key_lhs, TEST_NODE(rhs)->id, pdir);
}

results dowavl_base(RGB_Image *in_img)
{
    clock_t start, stop;
    double addtime, counttime;
    results res;
    start = clock();
    struct wavl_tree tree;
    wavl_tree_init(&tree, _test_node_to_node_compare_func, _test_node_to_value_compare_func);
    struct test_node *insertions = malloc(sizeof *insertions * in_img->size);
    RGB_Pixel *pixel;
    for (int i = 0; i < in_img->size; i++)
    {
        pixel = &in_img->data[i];
        uint key = (pixel->red << 16) | (pixel->green << 8) | pixel->blue;
        insertions[i].id = (int)key;
        // printf("%d\n", insertions[i].id);
        wavl_tree_insert(&tree, &insertions[i].id, &insertions[i].node);      
    }
    stop = clock();
    addtime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    start = clock();
    int num_cols = tree.count;
    stop = clock();
    counttime = ((double)(stop - start)) / CLOCKS_PER_SEC;
    res.num_cols = num_cols;
    res.addtime = addtime;
    res.counttime = counttime;
    free(insertions);
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
        results res = dowavl_base(in_img);
        totaladd += res.addtime;
        totalcount += res.counttime;
        num_cols = res.num_cols;
    }
    averageadd = totaladd / num_runs;
    averagecount = totalcount / num_runs;
    printf("Average time to add colors over %d runs: %f", num_runs ,averageadd);
    printf("\nAverage time to count colors over %d runs: %f", num_runs, averagecount);
    printf("\nNumber of unique colors: %d",num_cols);
    return 0;
}