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
    "foreskins",              /* Team name */

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
   
	int i, j,p,k;
	p = (dim - 1)*dim;
    for (i = 0; i < dim; i+= 16){
    	k = p;
    	for (j = 0; j < dim; j+= 1){
		dst[k] = src[RIDX(i, j, dim)];
		dst[k+1] = src[RIDX(i+1, j, dim)];
		dst[k+2] = src[RIDX(i+2, j, dim)];
		dst[k+3] = src[RIDX(i+3, j, dim)];
		dst[k+4] = src[RIDX(i+4, j, dim)];
		dst[k+5] = src[RIDX(i+5, j, dim)];
		dst[k+6] = src[RIDX(i+6, j, dim)];
		dst[k+7] = src[RIDX(i+7, j, dim)];
		dst[k+8] = src[RIDX(i+8, j, dim)];
		dst[k+9] = src[RIDX(i+9, j, dim)];
		dst[k+10] = src[RIDX(i+10, j, dim)];
		dst[k+11] = src[RIDX(i+11, j, dim)];
		dst[k+12] = src[RIDX(i+12, j, dim)];
		dst[k+13] = src[RIDX(i+13, j, dim)];
		dst[k+14] = src[RIDX(i+14, j, dim)];
		dst[k+15] = src[RIDX(i+15, j, dim)];
		k -= dim;
		}
		p += 16;
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
static void sun_coloums(pixel_sum *sum, pixel_sum *col1,pixel_sum *col2,pixel_sum *col3) 
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

	int i, j, p,k;
    pixel_sum col1;
    pixel_sum col2;
    pixel_sum col3;
    pixel_sum sum;
    initialize_pixel_sum(&col1);
    initialize_pixel_sum(&col2);
    initialize_pixel_sum(&col3);
    initialize_pixel_sum(&sum);
    for (i = 0; i < dim; i++){
    	p=i*dim;
    	initialize_pixel_sum(&col1);
    	initialize_pixel_sum(&col2);
    	initialize_pixel_sum(&col3);
		for (j = 0; j < dim; j++){
			k = p+j;
			
			//row 1
			if(i-1>=0){
    			if (j-1 >= 0)
    			{
    				if (col2.num == 0)
    				{	
    					accumulate_sum(&col1, src[k-dim-1]);
    				} else{
    					col1 = col2;
    				}
    			}
				if (col3.num == 0)
    				{	
    					accumulate_sum(&col2, src[k-dim]);
    				} else{
    					col2 = col3;
    				}
    			
    			if (j+1< dim)
    			{
    				accumulate_sum(&col3, src[k-dim+1]);

    			}
    		}



    		//row 2
			if (j-1 >= 0)
    		{
    			if (col2.num == 0)
    			{	
    				accumulate_sum(&col1, src[k-1]);
    			} else{
    				col1 = col2;
    			}
    			
    		}


    		if (col3.num == 0)
    		{	
    			accumulate_sum(&col2, src[k]);
    		} else{
    			col2 = col3;
    		}

    		if (j+1< dim)
    		{
    			accumulate_sum(&col3, src[k+1]);
    		}


    		// row 3
    		if(i+1<dim){
    			
    			if (j-1 >= 0)
    			{
    				if (col2.num == 0)
    				{	
    					accumulate_sum(&col1, src[k+dim-1]);
    				} else{
    					col1 = col2;
    				}
    				
    			}
				if (col3.num == 0)
    			{	
    				accumulate_sum(&col2, src[k+dim]);
    			} else{
    				col2 = col3;
    			}
    			
    		
    			if (j+1< dim)
    			{
    				accumulate_sum(&col3, src[k+dim+1]);
    			}
    		}
    		sun_coloums(&sum,&col1,&col2,&col3);
    		assign_sum_to_pixel(&dst[k], sum);
    		initialize_pixel_sum(&sum);
		}
	}
}
/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
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
    //add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
    add_smooth_function(&smooth1, smooth1_descr);
    add_smooth_function(&smooth2, smooth2_descr);
}

