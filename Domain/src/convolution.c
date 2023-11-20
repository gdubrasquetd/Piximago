#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include <complex.h>
#include <fftw3.h>
#include <imago2.h>
#include <unistd.h>
#include "utils.h"
#include "fft.h"

// gcc convolution.c fft.c utils.c -limago -lfftw3 -lpng -ljpeg -lm -ldl -o ../build/convolution

// Gaussian Blur
int normalize1 = 256;
int kernel1[5][5] = {
    {1, 4, 6, 4, 1},
    {4, 16, 24, 16, 4},
    {6, 24, 36, 24, 16},
    {4, 16, 24, 16, 4},
    {1, 4, 6, 4, 1}
};

// Emboss Effect
int normalize2 = 1;
int kernel2[5][5] = {
    { 0, 0, 0, 0, 0},
    { 0, -2, -1, 0, 0 },
    { 0, -1, 1, 1, 0 },
    { 0, 0, 1, 2, 0 },
    { 0, 0, 0, 0, 0 }
};

// Sharpening Effect
int normalize3 = -256;
int kernel3[5][5] = {
    {1, 4, 6, 4, 1},
    {4, 16, 24, 16, 4},
    {6, 24, -476, 24, 6},
    {4, 16, 24, 16, 4},
    {1, 4, 6, 4, 1}
};

// Edge Enhancement Effect
int normalize4 = 1000;
int kernel4[5][5] = {
    {-3, -15, -23, -15, -3},
    {-15, -62, -93, -62, -15},
    {-23, -93, 1859, -93, -23},
    {-15, -62, -93, -62, -15},
    {-3, -15, -23, -15, -3}
};

// Edge Detection (Laplacian)
int normalize5 = 1;
int kernel5[5][5] = {
    {0, 0, -1, 0, 0},
    {0, -1, -2, -1, 0},
    {-1, -2, 16, -2, -1},
    {0, -1, -2, -1, 0},
    {0, 0, -1, 0, 0}
};

// Sobel Top
int normalize6 = 1;
int kernel6[5][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 1, 2, 1, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, -1, -2, -1, 0 },
    { 0, 0, 0, 0, 0 }
};

// Sobel Bot
int normalize7 = 1;
int kernel7[5][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, -1, -2, -1, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 1, 2, 1, 0 },
    { 0, 0, 0, 0, 0 }
};

// Sobel Right
int normalize8 = 1;
int kernel8[5][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, -1, 0, 1, 0 },
    { 0, -2, 0, 2, 0 },
    { 0, -1, 0, 1, 0 },
    { 0, 0, 0, 0, 0 }
};

// Sobel Left
int normalize9 = 1;
int kernel9[5][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 1, 0, -1, 0 },
    { 0, 2, 0, -2, 0 },
    { 0, 1, 0, -1, 0 },
    { 0, 0, 0, 0, 0 }
};

unsigned char *  apply_kernel_convolution(unsigned char* img, int width, int height, int kernel[5][5], int normalize){
    int tmp =0;
    for(int i = 0; i<5; i++){
        for(int j = 0; j<5; j++){
            tmp += kernel[i][j];
        }
    }

    int k_width = 5;
    int k_height = 5;

    // Creation of new image with a zero padding
    int new_width = width + 4;
    int new_height = height + 4;
    int new_size = new_width * new_height;
    int* padding_img = (int*)malloc(new_size * sizeof(int));
    for(int ind=0; ind < new_size; ind++){
        padding_img[ind] = 300;
    }

    // Copy original image in the new image
    for(int i=2; i < new_height-2; i++){
        for(int j=2; j < new_width-2; j++){
            padding_img[i*new_width+j] = (int) img[(i-2)*width+(j-2)];
        }
    }


    // Apply convolution kernel
    for(int i=2; i < new_height-2; i++){
        for(int j=2; j < new_width-2; j++){
            int pixels_sum = 0;
            int ind = i*new_width+j;
            for(int ki=0; ki < k_width; ki++){
                for(int kj=0; kj < k_height; kj++){
                    int pixel = (i+(ki-2))*new_width + (j+(kj-2));
                    if(padding_img[pixel] == 300){
                        pixels_sum += padding_img[ind]*kernel[ki][kj];
                    }else{
                        pixels_sum += padding_img[pixel]*kernel[ki][kj];
                    }
                }
            }

            printf("somme %d \n", tmp);
            printf("res %d \n", check_rgb_range(pixels_sum/normalize*1.0));
            img[(i-2)*width+(j-2)] = check_rgb_range(pixels_sum/normalize*1.0);
        }
    }    

    free(padding_img);
    return img;
}




unsigned char * process_channel(unsigned char *channel, int width, int height, int kernel_nb)
{
    // Apply seleceted kernel
    switch (kernel_nb) {
        case 0:
            return apply_kernel_convolution(channel, width, height, kernel1, normalize1);
            break;
        case 1:
            return apply_kernel_convolution(channel, width, height, kernel2, normalize2);
            break;
        case 2:
            return apply_kernel_convolution(channel, width, height, kernel3, normalize3);
            break;
        case 3:
            return apply_kernel_convolution(channel, width, height, kernel4, normalize4);
            break;
        case 4:
            return apply_kernel_convolution(channel, width, height, kernel5, normalize5);
            break;
        case 5:
            return apply_kernel_convolution(channel, width, height, kernel6, normalize6);
            break;
        case 6:
            return apply_kernel_convolution(channel, width, height, kernel7, normalize7);
            break;
        case 7:
            return apply_kernel_convolution(channel, width, height, kernel8, normalize8);
            break;
        case 8:
            return apply_kernel_convolution(channel, width, height, kernel9, normalize9);
            break;
        default:
            fprintf(stderr, "Wrong kernel argument");
            return NULL;
    }
}



int
main(int argc, char *argv[])
{
    if(argc != 4){
        fprintf(stderr, 
            "Bad Arguments Error\n "
            "The command must look like :\n"
            "./convolution input_file output_file kernel\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments
    char* input_file = argv[1];
    char* output_file = concat_char(RESULT_PATH, argv[2]);
    int kernel_nb = atoi(argv[3]);

    // Open input image
    struct img_pixmap *img = open_img_from_name(input_file);
    int width = img->width;
    int height = img->height;

    // Decompose image to color channels (R, G, B)
	unsigned char *R = channel_extract(img->pixels, width * height, 0);
	unsigned char *G = channel_extract(img->pixels, width * height, 1);
	unsigned char*B = channel_extract(img->pixels, width * height, 2);

    unsigned char *newR = process_channel(R, width, height, kernel_nb);
    unsigned char *newG = process_channel(G, width, height, kernel_nb);
    unsigned char *newB = process_channel(B, width, height, kernel_nb);

    // Recompose image from modified color channels (R, G, B)
    unsigned char *res = channel_compose(newR, newG, newB, width * height);
    img_save_pixels(output_file, res, width, height, IMG_FMT_RGB24);

    free(R);
    free(G);
    free(B);
	free(res);

    exit(EXIT_SUCCESS);
}

