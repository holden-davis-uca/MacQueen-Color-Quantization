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
#include "./lib/util.c"
#include "./lib/avl.c"
#include "./lib/rb.c"

// Individual implementations
#include "./implementations/1darray.c"
#include "./implementations/bst.c"
#include "./implementations/2dbst.c"
#include "./implementations/2dsll.c"
#include "./implementations/3darray.c"
#include "./implementations/avl_base.c"
#include "./implementations/hashtable.c"
#include "./implementations/rb_base.c"

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

    #ifndef MEM_USAGE
    fprintf(data,
            "%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s",
            "IMAGE",
            "ADD (RB)", "CNT (RB)", "CLR (RB)",
            "ADD (AVL)", "CNT (AVL)", "CLR (AVL)",
            "ADD (HT)", "CNT (HT)", "CLR (HT)",
            "ADD (2D SLL)", "CNT (2D SLL)", "CLR (2D SLL)",
            "ADD (2D BST)", "CNT(2D BST)", "CLR (2D BST)",
            "ADD (BST)", "CNT(BST)", "CLR (BST)",
            "ADD (3D)", "CNT (3D)", "CLR (3D)",
            "ADD (1D)", "CNT (1D)", "CLR (1D)");
    fclose(data);
    #else
        fprintf(data,
            "%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s%-25s",
            "IMAGE",
            "ADD (RB)", "CNT (RB)", "CLR (RB)", "MEM (RB)",
            "ADD (AVL)", "CNT (AVL)", "CLR (AVL)", "MEM (AVL)",
            "ADD (HT)", "CNT (HT)", "CLR (HT)", "MEM (HT)",
            "ADD (2D SLL)", "CNT (2D SLL)", "CLR (2D SLL)", "MEM (2D SLL)",
            "ADD (2D BST)", "CNT(2D BST)", "CLR (2D BST)", "MEM (2D BST)",
            "ADD (BST)", "CNT(BST)", "CLR (BST)", "MEM (BST)",
            "ADD (3D)", "CNT (3D)", "CLR (3D)",
            "ADD (1D)", "CNT (1D)", "CLR (1D)");
    fclose(data);
    #endif
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
        totaladdtime1d, totalCNTtime1d,
        totaladdtime2dbst, totalCNTtime2dbst,
        totaladdtime2dsll, totalCNTtime2dsll,
        totaladdtime3d, totalCNTtime3d,
        totaladdtimebst, totalCNTtimebst,
        totaladdtimeht, totalCNTtimeht,
        totaladdtimeavl, totalCNTtimeavl,
        totaladdtimerb, totalCNTtimerb;
    double
        averageaddtime1d, averageCNTtime1d,
        averageaddtime2dbst, averageCNTtime2dbst,
        averageaddtime2dsll, averageCNTtime2dsll,
        averageaddtime3d, averageCNTtime3d,
        averageaddtimebst, averageCNTtimebst,
        averageaddtimeht, averageCNTtimeht,
        averageaddtimeavl, averageCNTtimeavl,
        averageaddtimerb, averageCNTtimerb;
    int
        num_cols_1d,
        num_cols_bst,
        num_cols_2dbst,
        num_cols_2dsll,
        num_cols_3d,
        num_cols_ht,
        num_cols_avl,
        num_cols_rb;

    #ifdef MEM_USAGE
    long 
        total_mem_1d,
        total_mem_bst,
        total_mem_2dbst,
        total_mem_2dsll,
        total_mem_3d,
        total_mem_ht,
        total_mem_avl,
        total_mem_rb;
    #endif

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
            totalCNTtime1d += res.counttime;
            num_cols_1d = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_1d = res.total_mem;
            #endif
            
            if (debug)
                printf("Executing run %d of BST on %s...\n", i+1, in_file_name);
            res = dobst(in_img);
            totaladdtimebst += res.addtime;
            totalCNTtimebst += res.counttime;
            num_cols_bst = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_bst = res.total_mem;
            #endif
            
            if (debug)
                printf("Executing run %d of 2D BST on %s...\n", i+1, in_file_name);
            res = do2dbst(in_img);
            totaladdtime2dbst += res.addtime;
            totalCNTtime2dbst += res.counttime;
            num_cols_2dbst = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_2dbst = res.total_mem;
            #endif
            
            if (debug)
                printf("Executing run %d of 2D SLL on %s...\n", i+1, in_file_name);
            res = do2dsll(in_img);
            totaladdtime2dsll += res.addtime;
            totalCNTtime2dsll += res.counttime;
            num_cols_2dsll = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_2dsll = res.total_mem;
            #endif
            
            if (debug)
                printf("Executing run %d of 3D Array on %s...\n", i+1, in_file_name);
            res = do3darray(in_img);
            totaladdtime3d += res.addtime;
            totalCNTtime3d += res.counttime;
            num_cols_3d = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_3d = res.total_mem;
            #endif
            
            if (debug)
                printf("Executing run %d of Hashtable on %s...\n", i+1, in_file_name);
            res = dohashtable(in_img);
            totaladdtimeht += res.addtime;
            totalCNTtimeht += res.counttime;
            num_cols_ht = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_ht = res.total_mem;
            #endif

            if (debug)
                printf("Executing run %d of AVL Tree on %s...\n", i+1, in_file_name);
            res = doavl_base(in_img);
            totaladdtimeavl += res.addtime;
            totalCNTtimeavl += res.counttime;
            num_cols_avl = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_avl = res.total_mem;
            #endif
            
            if (debug)
                printf("Executing run %d of RB Tree on %s...\n", i+1, in_file_name);
            res = dorb_base(in_img);
            totaladdtimerb += res.addtime;
            totalCNTtimerb += res.counttime;
            num_cols_rb = res.num_cols;
            #ifdef MEM_USAGE
            total_mem_rb = res.total_mem;
            #endif

        }

        //Rounding down CNT times for implementations with O(1) (constant time) methods
        averageaddtime1d = totaladdtime1d / num_runs; 
        averageCNTtime1d = totalCNTtime1d / num_runs;
        averageaddtime2dbst = totaladdtime2dbst / num_runs; 
        averageCNTtime2dbst = totalCNTtime2dbst / num_runs;
        averageaddtime2dsll = totaladdtime2dsll / num_runs;
        averageCNTtime2dsll = totalCNTtime2dsll / num_runs;
        averageaddtime3d = totaladdtime3d / num_runs;
        averageCNTtime3d = totalCNTtime3d / num_runs;
        averageaddtimebst = totaladdtimebst / num_runs; 
        averageCNTtimebst = totalCNTtimebst / num_runs;
        averageaddtimeht = totaladdtimeht / num_runs; 
        averageCNTtimeht = totalCNTtimeht / num_runs;
        averageaddtimeavl = totaladdtimeavl / num_runs; 
        averageCNTtimeavl = totalCNTtimeavl / num_runs;
        averageaddtimerb = totaladdtimerb / num_runs; 
        averageCNTtimerb = totalCNTtimerb / num_runs;

        #ifndef MEM_USAGE
        fprintf(data, "\n");
        fprintf(data,
                "%-25s%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d%-25g%-25g%-25d",
                in_file_name,
                averageaddtimerb, round(averageCNTtimerb), num_cols_rb,
                averageaddtimeavl, round(averageCNTtimeavl), num_cols_avl,
                averageaddtimeht, round(averageCNTtimeht), num_cols_ht,
                averageaddtime2dsll, averageCNTtime2dsll, num_cols_2dsll,
                averageaddtime2dbst, averageCNTtime2dbst, num_cols_2dbst,
                averageaddtimebst, averageCNTtimebst, num_cols_bst,
                averageaddtime1d, averageCNTtime1d, num_cols_1d,
                averageaddtime3d, averageCNTtime3d, num_cols_3d);
        fclose(data);
        #else
        fprintf(data, "\n");
        fprintf(data,
                "%-25s%-25g%-25g%-25d%-25lu%-25g%-25g%-25d%-25lu%-25g%-25g%-25d%-25lu%-25g%-25g%-25d%-25lu%-25g%-25g%-25d%-25lu%-25g%-25g%-25d%-25lu%-25g%-25g%-25d%-25g%-25g%-25d",
                in_file_name,
                averageaddtimerb, round(averageCNTtimerb), num_cols_rb, total_mem_rb / num_cols_rb,
                averageaddtimeavl, round(averageCNTtimeavl), num_cols_avl, total_mem_avl / num_cols_avl,
                averageaddtimeht, round(averageCNTtimeht), num_cols_ht, total_mem_ht / num_cols_ht,
                averageaddtime2dsll, averageCNTtime2dsll, num_cols_2dsll, total_mem_2dsll / num_cols_2dsll,
                averageaddtime2dbst, averageCNTtime2dbst, num_cols_2dbst, total_mem_2dbst / num_cols_2dbst,
                averageaddtimebst, averageCNTtimebst, num_cols_bst, total_mem_bst / num_cols_bst,
                averageaddtime1d, averageCNTtime1d, num_cols_1d,
                averageaddtime3d, averageCNTtime3d, num_cols_3d);
        fclose(data);
        #endif
    }
    fullstop = clock();
    data = fopen("data.txt", "a");
    fprintf(data, "\n");
    fprintf(data, "\n%d runs took %g seconds\n", num_runs, ((double)(fullstop - fullstart) / CLOCKS_PER_SEC));
    fclose(data);
    
    printf("\nCheck data.txt for stats!\n\n");
    return EXIT_SUCCESS;
}