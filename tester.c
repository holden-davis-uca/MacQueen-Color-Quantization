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
#include "pavl.c"
#include "prb.c"

// Individual implementations
#include "1darray.c"
#include "bst.c"
#include "2dbst.c"
#include "2dsll.c"
#include "3darray.c"
#include "avl_base.c"
#include "hashtable.c"
#include "rb_base.c"
#include "avl_pp.c"
#include "rb_pp.c"


int main(int argc, char **argv)
{
    clock_t fullstart, fullstop;
    fullstart = clock();
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
            "%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s",
            "IMAGE",
            "ADD (RB PP)", "COUNT (RB PP)", "COLORS (RB PP)",
            "ADD (AVL PP)", "COUNT (AVL PP)", "COLORS (AVL PP)",
            "ADD (RB)", "COUNT (RB)", "COLORS (RB)",
            "ADD (AVL)", "COUNT (AVL)", "COLORS (AVL)",
            "ADD (Hash Table)", "COUNT (Hash Table)", "COLORS (Hash Table)",
            "ADD (2-D SLL)", "COUNT (2-D SLL)", "COLORS (2-D SLL)",
            "ADD (2-D BST)", "COUNT(2-D BST)", "COLORS (2-D BST)",
            "ADD (BST)", "COUNT(BST)", "COLORS (BST)",
            "ADD (3-D)", "COUNT (3-D)", "COLORS (3-D)",
            "ADD (1-D)", "COUNT (1-D)", "COLORS (1-D)");
    fclose(data);
    const char *pictures[16];
    //Arranged by image size from smallest to largest
    //https://faculty.uca.edu/ecelebi/eight_images.zip - chairs, dissolve, fruit, blur, pencils, hotair, balloons, palette
    //https://faculty.uca.edu/ecelebi/eight_more_images.zip - chart, bags, 1931, pigments, bloom, market, candy, wood

    pictures[0] = "./images/market.ppm";
    pictures[1] = "./images/chart.ppm";
    pictures[2] = "./images/1931.ppm";
    pictures[3] = "./images/pigments.ppm";
    pictures[4] = "./images/hotair.ppm";
    pictures[5] = "./images/fruit.ppm";
    pictures[6] = "./images/dissolve.ppm";
    pictures[7] = "./images/palette.ppm";
    pictures[8] = "./images/wood.ppm";
    pictures[9] = "./images/bloom.ppm";
    pictures[10] = "./images/bags.ppm";
    pictures[11] = "./images/pencils.ppm";
    pictures[12] = "./images/chairs.ppm";
    pictures[13] = "./images/blur.ppm";
    pictures[14] = "./images/balloons.ppm";
    pictures[15] = "./images/candy.ppm";

    double
        totaladdtime1d, totalcounttime1d,
        totaladdtime2dbst, totalcounttime2dbst,
        totaladdtime2dsll, totalcounttime2dsll,
        totaladdtime3d, totalcounttime3d,
        totaladdtimebst, totalcounttimebst,
        totaladdtimeht, totalcounttimeht,
        totaladdtimeavl, totalcounttimeavl,
        totaladdtimerb, totalcounttimerb,
        totaladdtimeavlpp, totalcounttimeavlpp,
        totaladdtimerbpp, totalcounttimerbpp;
    double
        averageaddtime1d, averagecounttime1d,
        averageaddtime2dbst, averagecounttime2dbst,
        averageaddtime2dsll, averagecounttime2dsll,
        averageaddtime3d, averagecounttime3d,
        averageaddtimebst, averagecounttimebst,
        averageaddtimeht, averagecounttimeht,
        averageaddtimeavl, averagecounttimeavl,
        averageaddtimerb, averagecounttimerb,
        averageaddtimeavlpp, averagecounttimeavlpp,
        averageaddtimerbpp, averagecounttimerbpp;
    int
        num_cols_1d,
        num_cols_bst,
        num_cols_2dbst,
        num_cols_2dsll,
        num_cols_3d,
        num_cols_ht,
        num_cols_avl,
        num_cols_rb,
        num_cols_avlpp,
        num_cols_rbpp;

    // PER IMAGE:
    for (int y = 0; y < 16; y++)
    {
        data = fopen("data.txt", "a");
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

            if (debug)
                printf("Executing run %d of AVL PP Tree on %s...\n", i+1, in_file_name);
            res = doavl_base(in_img);
            totaladdtimeavlpp += res.addtime;
            totalcounttimeavlpp += res.counttime;
            num_cols_avlpp = res.num_cols;
            
            if (debug)
                printf("Executing run %d of RB PP Tree on %s...\n", i+1, in_file_name);
            res = dorb_base(in_img);
            totaladdtimerbpp += res.addtime;
            totalcounttimerbpp += res.counttime;
            num_cols_rbpp = res.num_cols;
        }

        //Rounding down count times for implementations with O(1) (constant time) methods
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
        averageaddtimeavlpp = totaladdtimeavlpp / num_runs; 
        averagecounttimeavlpp = totalcounttimeavlpp / num_runs;
        averageaddtimerbpp = totaladdtimerbpp / num_runs; 
        averagecounttimerbpp = totalcounttimerbpp / num_runs;

        fprintf(data, "\n");
        fprintf(data,
                "%-25s%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d",
                in_file_name,
                averageaddtimerbpp, round(averagecounttimerbpp), num_cols_rbpp,
                averageaddtimeavlpp, round(averagecounttimeavlpp), num_cols_avlpp,
                averageaddtimerb, round(averagecounttimerb), num_cols_rb,
                averageaddtimeavl, round(averagecounttimeavl), num_cols_avl,
                averageaddtimeht, round(averagecounttimeht), num_cols_ht,
                averageaddtime2dsll, averagecounttime2dsll, num_cols_2dsll,
                averageaddtime2dbst, averagecounttime2dbst, num_cols_2dbst,
                averageaddtimebst, averagecounttimebst, num_cols_bst,
                averageaddtime1d, averagecounttime1d, num_cols_1d,
                averageaddtime3d, averagecounttime3d, num_cols_3d);
        fclose(data);
    }
    fullstop = clock();
    data = fopen("data.txt", "a");
    fprintf(data, "\n");
    fprintf(data, "\n%d runs took %g seconds\n", num_runs, ((double)(fullstop - fullstart) / CLOCKS_PER_SEC));
    fclose(data);
    
    printf("\nCheck data.txt for stats!\n\n");
    return EXIT_SUCCESS;
}