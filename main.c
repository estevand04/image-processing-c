//
//  main.c
//  projectChallenge
//
//  Created by Estevan DeVargas on 12/8/24.
//

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utils/stb_image_write.h"

struct filter{
    int size_x, size_y;
    float **kernel;
};

float ** create_multiarray(int w, int h, int* filter){
    float **img;
    if (filter!=NULL){
        h += *filter - 1;
        w += *filter - 1;
    }
    img = malloc(h*sizeof(float *));
    for (int i = 0; i < h; i++)
        img[i] = calloc(w, sizeof(float));
    return img;
}

void from1Dto2D(float** img, unsigned char *arr, int w, int h, int c){
    int index = 0; // this intialized the index when accessing the 1d array
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            img[i][j] = arr[index];// assign the value from the 1d array to now the 2d array
            index += c;//moves to the next pixel in the 1d array
        }
    }

    
    
}

void convolution(float **new, float **A, int w, int h, struct filter filter){
    int x = filter.size_x / 2;
    int y = filter.size_y / 2;
    //calculating the padding in the x and y dimension
    for (int i = y; i < h - y; i++)
    {
        for (int j = x; j < w - x; j++)
        {
            float sum = 0; //sum for convolution
            for (int k = 0; k < filter.size_y; k++)
                //apply the filter kernel
            {
                for (int n = 0; n < filter.size_x; n++)
                {
                    sum += A[i - y + k][j - x + n] *filter.kernel[k][n];
                }
            }
            new[i][j] = sum;//stores the sum in the output array
        }
    }
}

void blurring(float** new, float** img, int w, int h, int filter_size){
    struct filter filter_x, filter_y;
    filter_x.size_x = filter_size;
    filter_y.size_y = filter_size;
    filter_x.size_y = 1;
    filter_y.size_x = 1;

    filter_x.kernel = malloc(filter_x.size_y * sizeof(float *));
    filter_x.kernel[0] = malloc(filter_x.size_x * sizeof(float));

    filter_y.kernel = malloc(filter_y.size_y * sizeof(float *));
    for (int i = 0; i<filter_size; i++)
        filter_y.kernel[i] = malloc(filter_y.size_x * sizeof(float));

    struct filter blur_filter;
    //defines a square blur filter
            blur_filter.size_x = filter_size;
            blur_filter.size_y = filter_size;

            // Create a uniform averaging filter
            blur_filter.kernel = malloc(blur_filter.size_y * sizeof(float *));
            for (int i = 0; i < blur_filter.size_y; i++) {
                blur_filter.kernel[i] = malloc(blur_filter.size_x * sizeof(float));
                for (int j = 0; j < blur_filter.size_x; j++)
                    //initializes the kernel with uniform values
                {
                    blur_filter.kernel[i][j] = 1.0f / (filter_size * filter_size);
                }
            }

            convolution(new, img, w, h, blur_filter);
    //perform the convolutionusing the blur gilter

            
            for (int i = 0; i < blur_filter.size_y; i++) {
                free(blur_filter.kernel[i]);
            }
            free(blur_filter.kernel);
    //frees the memory allocated for the kernel filter
}

float * min_max(float **img, int w, int h){
    //function to get the minimum an maximum value of the image
    static float result[2]; //static array that will hold the min and max values
    result[0] = img[0][0]; // min
    result[1] = img[0][0]; // max
    
    for (int i =0; i < h; i++)
        //traverse the image to try to find min and max values
    {
        for (int j = 0; j < w; j++)
        {
            if (img[i][j] < result[0]) result[0] = img[i][j];
            if (img[i][j] > result[1]) result[1] = img[i][j];
        }
    }
    return result;
}
void save_figure(float ** img, int w, int h, char* name)
//function to save the processed image as a png or jpg file
{
    int size = w * h * 3;
    unsigned char *arr = calloc(size, sizeof(unsigned char));
    int idx = 0;
    for (int i=0; i<h; i++)
        for(int j=0; j<w; j++)
            for(int temp=0; temp<3; temp++){
                arr[idx] = img[i][j];
                idx++;
            }
    stbi_write_jpg(name, w, h, 3, arr, w * 3);
}

int main(void)
{
    int w, h, c; // the width, height, and channels
    char filename[] = "/Users/stevd04/Desktop/C/projectChallenge/projectChallenge/data/lena.png"; // Change this to try different images

    // Average filter

    unsigned char *arr = stbi_load(filename, &w, &h, &c, 0);
    //loads the image using the stb_image file
    if (!arr)
    {
        printf("error loading the image \n");
        return -1;
    }
    float **img = create_multiarray(w, h, NULL);
    //converts the image date into the a 2d float array
    from1Dto2D(img, arr, w, h, c);
    save_figure(img, w, h, "gray.jpg");
    //saving the grayscale image
    
    float **blur = create_multiarray(w, h, NULL);
    //applys the blur bilter to the image
    blurring(blur, img, w, h, 15);
    save_figure(blur, w, h, "blur.jpg");
    
}
