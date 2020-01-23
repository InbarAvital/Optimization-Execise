/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#define BLOCK_SIZE 32
#define BLOCK_SIZE2 16
/*
 * Please fill in the following team struct
 */
        team_t team = {
        "alpha bete gamma",              /* Team name */

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
char rotate1_descr[] = "rotate1: test1";
void rotate1(int dim, pixel *src, pixel *dst)
{
    int i, j,x,y;

    for (i = 0; i < dim; i+= BLOCK_SIZE){
        for (j = 0; j < dim; j+= BLOCK_SIZE){
            for (x = i; x < BLOCK_SIZE + i; x+= 1){
                for (y = j; y < BLOCK_SIZE + j; y+= 1){
                    dst[RIDX(dim-1-y, x, dim)] = src[RIDX(x, y, dim)];
                }
            }
        }
    }

}
char rotate2_descr[] = "rotate2: test2";
void rotate2(int dim, pixel *src, pixel *dst)
{

    int i, j,x,y;

    for (i = 0; i < dim; i+= BLOCK_SIZE){
        for (j = 0; j < dim; j+= BLOCK_SIZE){
            for (x = i; x < BLOCK_SIZE + i; x+= 1){
                for (y = j; y < BLOCK_SIZE + j; y+= 1){
                    dst[RIDX(dim-1-y, x, dim)] = src[RIDX(x, y, dim)];
                }
            }
        }
    }
}
char rotate3_descr[] = "rotate3: test3";
void rotate3(int dim, pixel *src, pixel *dst)
{

    int i, j, p, k, x;
    p = (dim - 1)*dim;
    for (i = 0; i < dim; i+= BLOCK_SIZE){
        k = p;
        x = dim * i;
        for (j = 0; j < dim; j+= 1){
            x += j;
            dst[k] = src[x];
            x+=dim;
            dst[k+1] = src[x];
            x+=dim;
            dst[k+2] = src[x];
            x+=dim;
            dst[k+3] = src[x];
            x+=dim;
            dst[k+4] = src[x];
            x+=dim;
            dst[k+5] = src[x];
            x+=dim;
            dst[k+6] = src[x];
            x+=dim;
            dst[k+7] = src[x];
            x+=dim;
            dst[k+8] = src[x];
            x+=dim;
            dst[k+9] = src[x];
            x+=dim;
            dst[k+10] = src[x];
            x+=dim;
            dst[k+11] = src[x];
            x+=dim;
            dst[k+12] = src[x];
            x+=dim;
            dst[k+13] = src[x];
            x+=dim;
            dst[k+14] = src[x];
            x+=dim;
            dst[k+15] = src[x];
            x+=dim;
            dst[k+16] = src[x];
            x+=dim;
            dst[k+17] = src[x];
            x+=dim;
            dst[k+18] = src[x];
            x+=dim;
            dst[k+19] = src[x];
            x+=dim;
            dst[k+20] = src[x];
            x+=dim;
            dst[k+21] = src[x];
            x+=dim;
            dst[k+22] = src[x];
            x+=dim;
            dst[k+23] = src[x];
            x+=dim;
            dst[k+24] = src[x];
            x+=dim;
            dst[k+25] = src[x];
            x+=dim;
            dst[k+26] = src[x];
            x+=dim;
            dst[k+27] = src[x];
            x+=dim;
            dst[k+28] = src[x];
            x+=dim;
            dst[k+29] = src[x];
            x+=dim;
            dst[k+30] = src[x];
            x+=dim;
            dst[k+31] = src[x];
            x -= j + dim*31;
            k -= dim;
        }
        p += BLOCK_SIZE;
    }
}
char rotate4_descr[] = "rotate4: test4";
void rotate4(int dim, pixel *src, pixel *dst)
{

    int i, j,w;

    for (i = 0; i < dim; i+= 16){
        for (j = 0; j < dim; j+= 1){
            int k = (dim-1-j)*dim + i;
            int k2 = i*dim + j;
            int a = 0;
            for (w = 0; w < 16; w++){
                //dst[k+w] = src[RIDX(i+w, j, dim)];
                dst[k+w] = src[k2+a];
                a += dim;
            }
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
    rotate3(dim, src, dst);
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
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
    add_rotate_function(&rotate1, rotate1_descr);
    add_rotate_function(&rotate2, rotate2_descr);
    add_rotate_function(&rotate3, rotate3_descr);
    add_rotate_function(&rotate4, rotate4_descr);
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
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    int y1 = min(i+1, dim-1);
    int x1 = min(j+1, dim-1);
    int y2 = max(i-1, 0);
    int x2 = max(j-1, 0);
    for(ii = y2; ii <= y1; ii++)
        for(jj = x2; jj <= x1; jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/
static void sum_coloums(pixel_sum *sum, pixel_sum *col1,pixel_sum *col2,pixel_sum *col3)
{
    sum->red = (int) col1->red+col2->red+col3->red;
    sum->green = (int) col1->green+col2->green+col3->green;
    sum->blue = (int) col1->blue+col2->blue+col3->blue;
    sum->num = col1->num+col2->num+col3->num;
    return;
}
/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++){
        for (j = 0; j < dim; j++){
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
            //printf("%d:%d:%d\n", dst[RIDX(i, j, dim)].red,dst[RIDX(i, j, dim)].green,dst[RIDX(i, j, dim)].blue);
        }
    }
}

char smooth1_descr[] = "smooth1";
void smooth1(int dim, pixel *src, pixel *dst)
{
    int i, j ;
    //pixel_sum last;
    pixel_sum sum;
    initialize_pixel_sum(&sum);
    for (i = 0; i < dim; i++){

        for (j = 0; j < dim; j++){

            if(i-1>=0){
                if (j-1 >= 0)
                {
                    accumulate_sum(&sum, src[RIDX(i-1, j-1, dim)]);
                }

                accumulate_sum(&sum, src[RIDX(i-1, j, dim)]);

                if (j+1< dim)
                {
                    accumulate_sum(&sum, src[RIDX(i-1, j+1, dim)]);

                }
            }

            if (j-1 >= 0)
            {
                accumulate_sum(&sum, src[RIDX(i, j-1, dim)]);
            }

            accumulate_sum(&sum, src[RIDX(i, j, dim)]);

            if (j+1< dim)
            {
                accumulate_sum(&sum, src[RIDX(i, j+1, dim)]);
            }
            if(i+1<dim){

                if (j-1 >= 0)
                {
                    accumulate_sum(&sum, src[RIDX(i+1, j-1, dim)]);
                }

                accumulate_sum(&sum, src[RIDX(i+1, j, dim)]);

                if (j+1< dim)
                {
                    accumulate_sum(&sum, src[RIDX(i+1, j+1, dim)]);
                }
            }
            assign_sum_to_pixel(&dst[RIDX(i, j, dim)], sum);
            initialize_pixel_sum(&sum);
        }
    }


}
char smooth2_descr[] = "smooth2";
void smooth2(int dim, pixel *src, pixel *dst)
{

    int i=0, j, iip, iim, im;
    pixel_sum col1, col2,col3,sum;
    initialize_pixel_sum(&col1);
    initialize_pixel_sum(&col2);
    initialize_pixel_sum(&col3);
    initialize_pixel_sum(&sum);
    //

    initialize_pixel_sum(&col1);
    accumulate_sum(&col2, src[0]);
    accumulate_sum(&col2, src[dim]);
    accumulate_sum(&col3, src[1]);
    accumulate_sum(&col3, src[dim+1]);
    for (j = 0; j < dim-2; j++){
        sum_coloums(&sum, &col1,&col2,&col3);
        assign_sum_to_pixel(&dst[j], sum);
        initialize_pixel_sum(&sum);
        col1 = col2;
        col2 = col3;
        initialize_pixel_sum(&col3);
        accumulate_sum(&col3, src[j+2]);
        accumulate_sum(&col3, src[dim+j+2]);
    }
    sum_coloums(&sum, &col1,&col2,&col3);
    assign_sum_to_pixel(&dst[dim-2], sum);
    initialize_pixel_sum(&sum);
    initialize_pixel_sum(&col1);
    sum_coloums(&sum, &col1,&col2,&col3);
    assign_sum_to_pixel(&dst[dim-1], sum);

    //
    for (i = 1; i < dim-1; i++){
        iip = (i+1)*dim;
        iim = (i-1)*dim;
        im = i*dim;
        initialize_pixel_sum(&col1);
        initialize_pixel_sum(&col2);
        initialize_pixel_sum(&col3);
        accumulate_sum(&col2, src[iim]);
        accumulate_sum(&col2, src[im]);
        accumulate_sum(&col2, src[iip]);
        accumulate_sum(&col3, src[iim + 1]);
        accumulate_sum(&col3, src[im + 1]);
        accumulate_sum(&col3, src[iip + 1]);

        for (j = 0; j < dim-2; j++){
            sum_coloums(&sum, &col1,&col2,&col3);
            assign_sum_to_pixel(&dst[im + j], sum);
            initialize_pixel_sum(&sum);
            col1 = col2;
            col2 = col3;
            initialize_pixel_sum(&col3);
            accumulate_sum(&col3, src[iim + j + 2]);
            accumulate_sum(&col3, src[im + j + 2]);
            accumulate_sum(&col3, src[iip + j + 2]);
        }
        sum_coloums(&sum, &col1,&col2,&col3);
        assign_sum_to_pixel(&dst[im + dim - 2], sum);
        initialize_pixel_sum(&sum);
        initialize_pixel_sum(&col1);
        sum_coloums(&sum, &col1,&col2,&col3);
        assign_sum_to_pixel(&dst[im + dim - 1], sum);
    }
    //


    initialize_pixel_sum(&col1);
    initialize_pixel_sum(&col2);
    initialize_pixel_sum(&col3);
    accumulate_sum(&col2, src[RIDX(dim-2, 0, dim)]);
    accumulate_sum(&col2, src[RIDX(dim-1, 0, dim)]);
    accumulate_sum(&col3, src[RIDX(dim-2, 1, dim)]);
    accumulate_sum(&col3, src[RIDX(dim-1, 1, dim)]);

    for (j = 0; j < dim-2; j++){
        sum_coloums(&sum, &col1,&col2,&col3);
        assign_sum_to_pixel(&dst[RIDX(dim-1, j, dim)], sum);
        initialize_pixel_sum(&sum);
        col1 = col2;
        col2 = col3;
        initialize_pixel_sum(&col3);
        accumulate_sum(&col3, src[RIDX(dim-2, j+2, dim)]);
        accumulate_sum(&col3, src[RIDX(dim-1, j+2, dim)]);
    }
    sum_coloums(&sum, &col1,&col2,&col3);
    assign_sum_to_pixel(&dst[RIDX(dim-1, dim-2, dim)], sum);
    initialize_pixel_sum(&sum);
    initialize_pixel_sum(&col1);
    sum_coloums(&sum, &col1,&col2,&col3);
    assign_sum_to_pixel(&dst[RIDX(dim-1, dim-1, dim)], sum);

    //
}
char smooth3_descr[] = "smooth3";
void smooth3(int dim, pixel *src, pixel *dst)
{

    int i=0, j, iip, iim, k, im,p,x,y,z;
    int col1 =0, col2 = 0,col3 = 0 ,sum;
    //red

    col2 = src[0].red;
    col3 = src[1].red;
    col2 += src[dim].red;
    col3 += src[dim+1].red;
    sum = col2 + col3;
    dst[0].red = sum/4;
    x=dim+1;
    for (j = 1; j < dim-1; j++){
        col1 = col2;
        col2 = col3;
        col3 = src[j+1].red;
        col3 += src[x+j].red;
        sum = col1 + col2 + col3;
        dst[j].red = sum/6;
        j++;
        col1 = col2;
        col2 = col3;
        col3 = src[j+1].red;
        col3 += src[x+j].red;
        sum = col1 + col2 + col3;
        dst[j].red = sum/6;
    }
    col1 = col2;
    col2 = col3;
    sum = col1 + col2;
    dst[dim-1].red = sum/4;

    //
    for (i = 1; i < dim-1; i++){
        iip = (i+1)*dim;
        iim = (i-1)*dim;
        im = i*dim;
        x = iim+1;
        y = im+1;
        z = iip+1;
        col2 = src[iim].red;
        col3 = src[x].red;
    	col2 += src[im].red;
    	col3 += src[y].red;
    	col2 += src[iip].red;
    	col3 += src[z].red;
        sum = col2 + col3;
    	dst[im].red = sum/6;

        for (j = 1; j < dim-1; j++){
            col1 = col2;
            col2 = col3;
            col3 = src[x+j].red;
        	col3 += src[y+j].red;
        	col3 += src[z+j].red;
        	sum = col1 + col2 + col3;
        	dst[im+j].red = sum/9;
        	j++;
			col1 = col2;
            col2 = col3;
            col3 = src[x+j].red;
        	col3 += src[y+j].red;
        	col3 += src[z+j].red;
        	sum = col1 + col2 + col3;
        	dst[im+j].red = sum/9;
        }
        col1 = col2;
        col2 = col3;
        sum = col1 + col2;
    	dst[im + dim-1].red = sum/6;
    }
    //
    k = (dim-1)*dim;
    p =(dim-2)*dim;
    x=p+1;
    y=k+1;
    col2 = src[p].red;
    col3 = src[x].red;
    col2 += src[k].red;
    col3 += src[y].red;
    sum = col2 + col3;
    dst[k].red = sum/4;

    for (j = 1; j < dim-1; j++){
        col1 = col2;
        col2 = col3;
        col3 = src[x+j].red;
        col3 += src[y+j].red;
        sum = col1 + col2 + col3;
        dst[k+j].red = sum/6;
        j++;
        col1 = col2;
        col2 = col3;
        col3 = src[x+j].red;
        col3 += src[y+j].red;
        sum = col1 + col2 + col3;
        dst[k+j].red = sum/6;
    }
    col1 = col2;
    col2 = col3;
    sum = col1 + col2;
    dst[k+dim-1].red = sum/4;

    //green
    col2 = src[0].green;
    col3 = src[1].green;
    col2 += src[dim].green;
    col3 += src[dim+1].green;
    sum = col2 + col3;
    dst[0].green = sum/4;
    x=dim+1;
    for (j = 1; j < dim-1; j++){
        col1 = col2;
        col2 = col3;
        col3 = src[j+1].green;
        col3 += src[x+j].green;
        sum = col1 + col2 + col3;
        dst[j].green = sum/6;
        j++;
        col1 = col2;
        col2 = col3;
        col3 = src[j+1].green;
        col3 += src[x+j].green;
        sum = col1 + col2 + col3;
        dst[j].green = sum/6;
    }
    col1 = col2;
    col2 = col3;
    sum = col1 + col2;
    dst[dim-1].green = sum/4;

    //
    for (i = 1; i < dim-1; i++){
        iip = (i+1)*dim;
        iim = (i-1)*dim;
        im = i*dim;
        x = iim+1;
        y = im+1;
        z = iip+1;
        col2 = src[iim].green;
        col3 = src[x].green;
    	col2 += src[im].green;
    	col3 += src[y].green;
    	col2 += src[iip].green;
    	col3 += src[z].green;
        sum = col2 + col3;
    	dst[im].green = sum/6;

        for (j = 1; j < dim-1; j++){
            col1 = col2;
            col2 = col3;
            col3 = src[x+j].green;
        	col3 += src[y+j].green;
        	col3 += src[z+j].green;
        	sum = col1 + col2 + col3;
        	dst[im+j].green = sum/9;
        	j++;
        	col1 = col2;
            col2 = col3;
            col3 = src[x+j].green;
        	col3 += src[y+j].green;
        	col3 += src[z+j].green;
        	sum = col1 + col2 + col3;
        	dst[im+j].green = sum/9;
        }
        col1 = col2;
        col2 = col3;
        sum = col1 + col2;
    	dst[im + dim-1].green = sum/6;
    }
    //
    k = (dim-1)*dim;
    p =(dim-2)*dim;
    x=p+1;
    y=k+1;
    col2 = src[p].green;
    col3 = src[x].green;
    col2 += src[k].green;
    col3 += src[y].green;
    sum = col2 + col3;
    dst[k].green = sum/4;

    for (j = 1; j < dim-1; j++){
        col1 = col2;
        col2 = col3;
        col3 = src[x+j].green;
        col3 += src[y+j].green;
        sum = col1 + col2 + col3;
        dst[k+j].green = sum/6;
        j++;
        col1 = col2;
        col2 = col3;
        col3 = src[x+j].green;
        col3 += src[y+j].green;
        sum = col1 + col2 + col3;
        dst[k+j].green = sum/6;
    }
    col1 = col2;
    col2 = col3;
    sum = col1 + col2;
    dst[k+dim-1].green = sum/4;
    //blue
        col2 = src[0].blue;
    col3 = src[1].blue;
    col2 += src[dim].blue;
    col3 += src[dim+1].blue;
    sum = col2 + col3;
    dst[0].blue = sum/4;
    x=dim+1;
    for (j = 1; j < dim-1; j++){
        col1 = col2;
        col2 = col3;
        col3 = src[j+1].blue;
        col3 += src[x+j].blue;
        sum = col1 + col2 + col3;
        dst[j].blue = sum/6;
        j++;
        col1 = col2;
        col2 = col3;
        col3 = src[j+1].blue;
        col3 += src[x+j].blue;
        sum = col1 + col2 + col3;
        dst[j].blue = sum/6;
    }
    col1 = col2;
    col2 = col3;
    sum = col1 + col2;
    dst[dim-1].blue = sum/4;

    //
    for (i = 1; i < dim-1; i++){
        iip = (i+1)*dim;
        iim = (i-1)*dim;
        im = i*dim;
        x = iim+1;
        y = im+1;
        z = iip+1;
        col2 = src[iim].blue;
        col3 = src[x].blue;
    	col2 += src[im].blue;
    	col3 += src[y].blue;
    	col2 += src[iip].blue;
    	col3 += src[z].blue;
        sum = col2 + col3;
    	dst[im].blue = sum/6;

        for (j = 1; j < dim-1; j++){
            col1 = col2;
            col2 = col3;
            col3 = src[x+j].blue;
        	col3 += src[y+j].blue;
        	col3 += src[z+j].blue;
        	sum = col1 + col2 + col3;
        	dst[im+j].blue = sum/9;
        	j++;
        	col1 = col2;
            col2 = col3;
            col3 = src[x+j].blue;
        	col3 += src[y+j].blue;
        	col3 += src[z+j].blue;
        	sum = col1 + col2 + col3;
        	dst[im+j].blue = sum/9;
        }
        col1 = col2;
        col2 = col3;
        sum = col1 + col2;
    	dst[im + dim-1].blue = sum/6;
    }
    //
    k = (dim-1)*dim;
    p =(dim-2)*dim;
    x=p+1;
    y=k+1;
    col2 = src[p].blue;
    col3 = src[x].blue;
    col2 += src[k].blue;
    col3 += src[y].blue;
    sum = col2 + col3;
    dst[k].blue = sum/4;

    for (j = 1; j < dim-1; j++){
        col1 = col2;
        col2 = col3;
        col3 = src[x+j].blue;
        col3 += src[y+j].blue;
        sum = col1 + col2 + col3;
        dst[k+j].blue = sum/6;
        j++;
        col1 = col2;
        col2 = col3;
        col3 = src[x+j].blue;
        col3 += src[y+j].blue;
        sum = col1 + col2 + col3;
        dst[k+j].blue = sum/6;
    }
    col1 = col2;
    col2 = col3;
    sum = col1 + col2;
    dst[k+dim-1].blue = sum/4;
}
/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst)
{
    smooth3(dim, src, dst);
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
    add_smooth_function(&smooth1, smooth1_descr);
    add_smooth_function(&smooth2, smooth2_descr);
    add_smooth_function(&smooth3, smooth3_descr);
}
