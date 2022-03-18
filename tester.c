/*

To compile: make %tester%

To run: ./%tester% -r %NUMBER_OF_RUNS%

It can be run with no arguments and the number of runs will default to 1

*/
// Generic requirements
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "util.c"
#include "avl.c"
#include "rb.c"

// Individual implementations
#include "1darray.c"
#include "bst.c"
#include "2dbst.c"
#include "2dsll.c"
#include "3darray.c"
#include "avl_base.c"
#include "hashtable.c"
#include "rb_base.c"

int main(int argc, char **argv)
{
    // Everything that is only done once: ie. things that executed a single time regardless of number of runs
    int num_runs = 1;
    bool debug = false;

    if (argc == 1)
    {
        print_usage_test(argv[0]);
    }
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-r"))
        {
            num_runs = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-d"))
        {
            debug = true;
        }
        else
        {
            print_usage_test(argv[0]);
        }
    }
    FILE *data;
    data = fopen("data.txt", "w");

    fprintf(data, "ALL TIMING DATA AVERAGED OVER %d RUNS. PLEASE REPORT ANY ERRORS TO https://github.com/holden-davis-uca/Research\n\n", num_runs);

    fprintf(data,
            "%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s",
            "IMAGE",
            "ADD (RB)", "COUNT (RB)", "COLORS (RB)",
            "ADD (AVL)", "COUNT (AVL)", "COLORS (AVL)",
            "ADD (Hash Table)", "COUNT (Hash Table)", "COLORS (Hash Table)",
            "ADD (2-D SLL)", "COUNT (2-D SLL)", "COLORS (2-D SLL)",
            "ADD (2-D BST)", "COUNT(2-D BST)", "COLORS (2-D BST)",
            "ADD (BST)", "COUNT(BST)", "COLORS (BST)",
            "ADD (3-D)", "COUNT (3-D)", "COLORS (3-D)",
            "ADD (1-D)", "COUNT (1-D)", "COLORS (1-D)");

    const char *pictures[10];
    pictures[0] = "./images/baboon.ppm";
    pictures[1] = "./images/fish.ppm";
    pictures[2] = "./images/girl.ppm";
    pictures[3] = "./images/goldhill.ppm";
    pictures[4] = "./images/kodim05.ppm";
    pictures[5] = "./images/kodim23.ppm";
    pictures[6] = "./images/peppers.ppm";
    pictures[7] = "./images/pills.ppm";
    pictures[8] = "./images/frymire.ppm";
    pictures[9] = "./images/kiss.ppm";
    double
        totaladdtime1d, totalcounttime1d,
        totaladdtime2dbst, totalcounttime2dbst,
        totaladdtime2dsll, totalcounttime2dsll,
        totaladdtime3d, totalcounttime3d,
        totaladdtimebst, totalcounttimebst,
        totaladdtimeht, totalcounttimeht,
        totaladdtimeavl, totalcounttimeavl,
        totaladdtimerb, totalcounttimerb;
    double
        averageaddtime1d, averagecounttime1d,
        averageaddtime2dbst, averagecounttime2dbst,
        averageaddtime2dsll, averagecounttime2dsll,
        averageaddtime3d, averagecounttime3d,
        averageaddtimebst, averagecounttimebst,
        averageaddtimeht, averagecounttimeht,
        averageaddtimeavl, averagecounttimeavl,
        averageaddtimerb, averagecounttimerb;
    int
        num_cols_1d,
        num_cols_bst,
        num_cols_2dbst,
        num_cols_2dsll,
        num_cols_3d,
        num_cols_ht,
        num_cols_avl,
        num_cols_rb;

    // PER IMAGE:
    for (int y = 0; y < 10; y++)
    {
        char in_file_name[256];
        RGB_Image *in_img;
        results res;
        strcpy(in_file_name, pictures[y]);
        in_img = read_PPM(in_file_name);
        // PER RUN:
        for (int i = 0; i < num_runs; i++)
        {
            if (debug)
                printf("Executing run %d of 1D Array on %s...\n", i+1, in_file_name);
            res = do1darray(in_img);
            totaladdtime1d += res.addtime;
            totalcounttime1d += res.counttime;
            num_cols_1d = res.num_cols;
            
            if (debug)
                printf("Executing run %d of BST on %s...\n", i+1, in_file_name);
            res = dobst(in_img);
            totaladdtimebst += res.addtime;
            totalcounttimebst += res.counttime;
            num_cols_bst = res.num_cols;
            
            if (debug)
                printf("Executing run %d of 2D BST on %s...\n", i+1, in_file_name);
            res = do2dbst(in_img);
            totaladdtime2dbst += res.addtime;
            totalcounttime2dbst += res.counttime;
            num_cols_2dbst = res.num_cols;
            
            if (debug)
                printf("Executing run %d of 2D SLL on %s...\n", i+1, in_file_name);
            res = do2dsll(in_img);
            totaladdtime2dsll += res.addtime;
            totalcounttime2dsll += res.counttime;
            num_cols_2dsll = res.num_cols;
            
            if (debug)
                printf("Executing run %d of 3D Array on %s...\n", i+1, in_file_name);
            res = do3darray(in_img);
            totaladdtime3d += res.addtime;
            totalcounttime3d += res.counttime;
            num_cols_3d = res.num_cols;
            
            if (debug)
                printf("Executing run %d of Hashtable on %s...\n", i+1, in_file_name);
            res = dohashtable(in_img);
            totaladdtimeht += res.addtime;
            totalcounttimeht += res.counttime;
            num_cols_ht = res.num_cols;
            
            if (debug)
                printf("Executing run %d of AVL Tree on %s...\n", i+1, in_file_name);
            res = doavl_base(in_img);
            totaladdtimeavl += res.addtime;
            totalcounttimeavl += res.counttime;
            num_cols_avl = res.num_cols;
            
            if (debug)
                printf("Executing run %d of RB Tree on %s...\n", i+1, in_file_name);
            res = dorb_base(in_img);
            totaladdtimerb += res.addtime;
            totalcounttimerb += res.counttime;
            num_cols_rb = res.num_cols;
        }
        averageaddtime1d = totaladdtime1d / num_runs; 
        averagecounttime1d = totalcounttime1d / num_runs;
        averageaddtime2dbst = totaladdtime2dbst / num_runs; 
        averagecounttime2dbst = totalcounttime2dbst / num_runs;
        averageaddtime2dsll = totaladdtime2dsll / num_runs;
        averagecounttime2dsll = totalcounttime2dsll / num_runs;
        averageaddtime3d = totaladdtime3d / num_runs;
        averagecounttime3d = totalcounttime3d / num_runs;
        averageaddtimebst = totaladdtimebst / num_runs; 
        averagecounttimebst = totalcounttimebst / num_runs;
        averageaddtimeht = totaladdtimeht / num_runs; 
        averagecounttimeht = totalcounttimeht / num_runs;
        averageaddtimeavl = totaladdtimeavl / num_runs; 
        averagecounttimeavl = totalcounttimeavl / num_runs;
        averageaddtimerb = totaladdtimerb / num_runs; 
        averagecounttimerb = totalcounttimerb / num_runs;

        fprintf(data, "\n");
        fprintf(data,
                "%-25s%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d",
                in_file_name,
                averageaddtimerb, averagecounttimerb, num_cols_rb,
                averageaddtimeavl, averagecounttimeavl, num_cols_avl,
                averageaddtimeht, averagecounttimeht, num_cols_ht,
                averageaddtime2dsll, averagecounttime2dsll, num_cols_2dsll,
                averageaddtime2dbst, averagecounttime2dbst, num_cols_2dbst,
                averageaddtimebst, averagecounttimebst, num_cols_bst,
                averageaddtime1d, averagecounttime1d, num_cols_1d,
                averageaddtime3d, averagecounttime3d, num_cols_3d);
    }

    fclose(data);
    printf("\nCheck data.txt for stats!\n\n");
    return EXIT_SUCCESS;
}