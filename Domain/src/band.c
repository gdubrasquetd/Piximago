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

// gcc band.c fft.c utils.c -limago -lfftw3 -lpng -ljpeg -lm -ldl -o ../build/band

void cut_band(double *spectrum, int width, int height, int min, int max, int mode) {
    double *mask = (double *)malloc(width * height * sizeof(double));
    memset(mask, 1.0, width * height * sizeof(double));
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
            int dist = sqrt(pow(i - (height/2), 2) + pow(j - (width/2), 2));
            if(mode == 0){
                if(dist < min || dist > max){
                    spectrum[ind] = 0.0;
                    mask[ind] = 0.0;
                }
            }else if(mode == 1) {
                if(dist >= min && dist <= max){
                    spectrum[ind] = 0.0;
                    mask[ind] = 0.0;
                }
            }
        }
    }
    // Save modified each channel mask to png
    save_image_double(mask, width, height, MASK_PATH,  concat_char(RGB[RGBcpt], "mask.png"));
    free(mask);
}


static unsigned char *process_channel(unsigned char *channel, int width, int height, int min, int max, int mode)
{
    // Extracting spectrum using Fourier transform
    fftw_complex *fr_channel = fft_forward(channel, width, height);
    double *as = fft_extract_as(fr_channel, width, height);
    double *ps = fft_extract_ps(fr_channel, width, height);

    cut_band(as, width, height, min, max, mode);
    
    // Save modified each channel spectrum to png
    save_image_double(as, width, height, SPECTRUM_PATH,  concat_char(RGB[RGBcpt], "spectrum.png"));
    RGBcpt++;

    fftw_complex *fr_result = fft_compose_aps(as, ps, width, height);
    unsigned char *result = fft_backward(fr_result, width, height);

    fftw_free(fr_channel);
    free(as);
    free(ps);
   	fftw_free(fr_result);

    return result;
}


int main(int argc, char *argv[])
{
    if(argc != 6){
        fprintf(stderr, 
            "Bad Arguments Error\n "
            "The command must look like :\n"
            "./band input_file output_file min max mode\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments
    char *input_file = argv[1];
    char *output_file = concat_char(RESULT_PATH, argv[2]);
    int min = atoi(argv[3]);
    int max = atoi(argv[4]);
    int mode = atoi(argv[5]);

    // Open input image
    struct img_pixmap *img = open_img_from_name(input_file);
    int width = img->width;
    int height = img->height;

    int max_radius = sqrt(pow((height/2), 2) + pow((width/2), 2));
    max = max_radius * (max/100.0);
    min = max_radius * (min/100.0);

    // Decompose image to color channels (R, G, B)
	unsigned char *R = channel_extract(img->pixels, width * height, 0);
	unsigned char *G = channel_extract(img->pixels, width * height, 1);
	unsigned char*B = channel_extract(img->pixels, width * height, 2);

    int newWidth = 0;
    int newHeight = 0;

    // Normalize image size
    unsigned char *resized_R = normalizeImageDimension(R, width, height, &newWidth, &newHeight, 900);
    unsigned char *resized_G = normalizeImageDimension(G, width, height, &newWidth, &newHeight, 900);
    unsigned char *resized_B = normalizeImageDimension(B, width, height, &newWidth, &newHeight, 900);

    unsigned char *newR = process_channel(resized_R, newWidth, newHeight, min, max, mode);
    unsigned char *newG = process_channel(resized_G, newWidth, newHeight, min, max, mode);
    unsigned char *newB = process_channel(resized_B, newWidth, newHeight, min, max, mode);

    // Recompose image from modified color channels (R, G, B)
    unsigned char *res = channel_compose(newR, newG, newB, newWidth * newHeight);
    img_save_pixels(output_file, res, newWidth, newHeight, IMG_FMT_RGB24);
    
    free(resized_R);
    free(resized_G);
    free(resized_B);
    free(newR);
    free(newG);
    free(newB);
    
    exit(EXIT_SUCCESS);
}

