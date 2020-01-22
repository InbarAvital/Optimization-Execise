/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#define BLOCK_SIZE 16


void transpose(int i, int j, pixel* src, pixel* dst, int dim);

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Inbar",              /* Team name */

    "Inbar Avital",     /* First member full name */
    "inbaravital4@gmail.com",  /* First member email address */

    "Shachar Meir",                   /* Second member full name (leave blank if none) */
    "shachar.meir007@gmail.com"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*                       
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
	int i, j;
    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];          
}


char loop_unrolling_rotate_descr[] = "loop_unrolling_rotate: try to unroll";
void loop_unrolling_rotate(int dim, pixel* src, pixel* dst)
{
    int i, j;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j+=2) {
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(dim - 2 - j, i, dim)] = src[RIDX(i, j + 1, dim)];
        }
    }
}

char rotate1_descr[] = "rotate1: test 1";
void rotate1(int dim, pixel* src, pixel* dst)
{
    int i, j, x, y;
    for (i = 0; i < dim; i += BLOCK_SIZE) {
        for (j = 0; j < dim; j += BLOCK_SIZE) {
            int x_max_size = BLOCK_SIZE + i;
            int y_max_size = BLOCK_SIZE + j;
            //block
            for (x = i; x < x_max_size; x++) {
                y = j;
                int src_ups = x * dim + y;
                dst[RIDX(dim - 1 - y, x, dim)] = src[src_ups];
                dst[RIDX(dim - 2 - y, x, dim)] = src[src_ups+1];
                dst[RIDX(dim - 3 - y, x, dim)] = src[src_ups+2];
                dst[RIDX(dim - 4 - y, x, dim)] = src[src_ups+3];
                dst[RIDX(dim - 5 - y, x, dim)] = src[src_ups+4];
                dst[RIDX(dim - 6 - y, x, dim)] = src[src_ups+5];
                dst[RIDX(dim - 7 - y, x, dim)] = src[src_ups+6];
                dst[RIDX(dim - 8 - y, x, dim)] = src[src_ups+7];
                dst[RIDX(dim - 9 - y, x, dim)] = src[src_ups+8];
                dst[RIDX(dim - 10 - y, x, dim)] = src[src_ups+9];
                dst[RIDX(dim - 11 - y, x, dim)] = src[src_ups+10];
                dst[RIDX(dim - 12 - y, x, dim)] = src[src_ups+11];
                dst[RIDX(dim - 13 - y, x, dim)] = src[src_ups+12];
                dst[RIDX(dim - 14 - y, x, dim)] = src[src_ups+13];
                dst[RIDX(dim - 15 - y, x, dim)] = src[src_ups+14];
                dst[RIDX(dim - 16 - y, x, dim)] = src[src_ups];
            }
        }
    }
}

char rotate2_descr[] = "Loop unrolling by 16, 2.6";
void rotate2(int dim, pixel* src, pixel* dst) {
    int i, j, k;

    for (i = 0; i < dim; i += 16) {
        for (j = 0; j < dim; j++) {
            k = (dim - 1 - j) * dim + i;

            dst[k] = src[i * dim + j];
            dst[k + 1] = src[(i + 1) * dim + j];
            dst[k + 2] = src[(i + 2) * dim + j];
            dst[k + 3] = src[(i + 3) * dim + j];
            dst[k + 4] = src[(i + 4) * dim + j];
            dst[k + 5] = src[(i + 5) * dim + j];
            dst[k + 6] = src[(i + 6) * dim + j];
            dst[k + 7] = src[(i + 7) * dim + j];
            dst[k + 8] = src[(i + 8) * dim + j];
            dst[k + 9] = src[(i + 9) * dim + j];
            dst[k + 10] = src[(i + 10) * dim + j];
            dst[k + 11] = src[(i + 11) * dim + j];
            dst[k + 12] = src[(i + 12) * dim + j];
            dst[k + 13] = src[(i + 13) * dim + j];
            dst[k + 14] = src[(i + 14) * dim + j];
            dst[k + 15] = src[(i + 15) * dim + j];
        }
    }
}

char rotate3_descr[] = "rotate1: test 1";
void rotate3(int dim, pixel* src, pixel* dst)
{
    int i, j, k, k2;

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j += 16) {
            k2 = i * dim + i;
            dst[j * dim + i] = src[k2];
            dst[k + 1] = src[k2 + 1];
            dst[k + 2] = src[k2 + 2];
            dst[k + 3] = src[k2 + 3];
            dst[k + 4] = src[k2 + 4];
            dst[k + 5] = src[k2 + 5];
            dst[k + 6] = src[k2 + 6];
            dst[k + 7] = src[k2 + 7];
            dst[k + 8] = src[k2 + 8];
            dst[k + 9] = src[k2 + 9];
            dst[k + 10] = src[k2 + 10];
            dst[k + 11] = src[k2 + 11];
            dst[k + 12] = src[k2 + 12];
            dst[k + 13] = src[k2 + 13];
            dst[k + 14] = src[k2 + 14];
            dst[k + 15] = src[k2 + 15];
        }
    }
}
/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j, x, y;
    for (i = 0; i < dim; i += BLOCK_SIZE) {
        for (j = 0; j < dim; j += BLOCK_SIZE) {
            int x_max_size = BLOCK_SIZE + i;
            int y_max_size = BLOCK_SIZE + j;
            //block
            for (x = i; x < x_max_size; x++) {
                for (y = j; y < y_max_size; y++) {
                    dst[RIDX(dim - 1 - y, x, dim)] = src[RIDX(x, y, dim)];
                }
            }
        }
    }
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&loop_unrolling_rotate, loop_unrolling_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&rotate1, rotate1_descr);
    add_rotate_function(&rotate2, rotate2_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

 /***************************************************************
  * Various typedefs and helper functions for the smooth function
  * You may modify these any way you like.
  **************************************************************/

  /* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum* sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum* sum, pixel p)
{
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel* current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel* src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

 /*
  * naive_smooth - The naive baseline version of smooth
  */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel* src, pixel* dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel* src, pixel* dst)
{
    naive_smooth(dim, src, dst);
}


/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

