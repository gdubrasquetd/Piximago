#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include <complex.h>
#include <fftw3.h>
#include <imago2.h>
#include <unistd.h>

#include "fft.h"

#include "utils.h"

// gcc cross.c fft.c utils.c -limago -lfftw3 -lpng -ljpeg -lm -ldl -o ../build/cross
// ./cross image radius center mode


void cut_cross(double *spectrum, int width, int height, int radius, int center, int mode, float position) {
    double *mask = (double *)malloc(width * height * sizeof(double));
    memset(mask, 1.0, width * height * sizeof(double));

    if(mode == 0 || mode == 1){
        // Draw a half-line from the top to the middle
        for(int i=0; i < height/2 - center; i++) {
            for(int j=width * position - radius; j < width * position + radius; j++) {
                int ind = i*width + j;
                spectrum[ind] = 0.0;
                mask[ind] = 0.0;
            }
        }
        // Draw a half-line from the middle to the bottom
        for(int i=height/2 + center ; i < height; i++) {
            for(int j=width * position - radius; j < width * position + radius; j++) {
                int ind = i*width + j;
                spectrum[ind] = 0.0;
                mask[ind] = 0.0;
            }
        }
    }

    if(mode == 0 || mode == 2){
        // Draw a half-line from the left to the middle
        for(int i=height * position - radius; i < height * position + radius; i++) {
            for(int j=0; j < width/2 - center; j++) {
                int ind = i*width + j;
                spectrum[ind] = 0.0;
                mask[ind] = 0.0;
            }
        }
        // Draw a half-line from the middle to the right
        for(int i=height * position - radius; i < height * position + radius; i++) {
            for(int j=width/2 + center; j < width; j++) {
                int ind = i*width + j;
                spectrum[ind] = 0.0;
                mask[ind] = 0.0;
            }
        }
    }
    // Save modified each channel mask to png
    save_image_double(mask, width, height, MASK_PATH,  concat_char(RGB[RGBcpt], "mask.png"));
    free(mask);
}


static unsigned char *process_channel(unsigned char *channel, int width, int height, int radius, int center, int mode, float position)
{
    // Extracting spectrum using Fourier transform
    fftw_complex *fr_channel = fft_forward(channel, width, height);
    double *as = fft_extract_as(fr_channel, width, height);
    double *ps = fft_extract_ps(fr_channel, width, height);

    cut_cross(as, width, height, radius, center, mode, position);

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
    if(argc != 7){
        fprintf(stderr, 
            "Bad Arguments Error\n "
            "The command must look like :\n"
            "./cross input_file output_file radius center mode position\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments
    char* input_file = argv[1];
    char* output_file = concat_char(RESULT_PATH, argv[2]);
    int radius = atoi(argv[3]);
    int center = atoi(argv[4]);
    int mode = atoi(argv[5]);
    float position = atoi(argv[6])/100.0;

    // Open input image
    struct img_pixmap *img = open_img_from_name(input_file);
    int width = img->width;
    int height = img->height;

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

    unsigned char *newR = process_channel(resized_R, newWidth, newHeight, radius, center, mode, position);
    unsigned char *newG = process_channel(resized_G, newWidth, newHeight, radius, center, mode, position);
    unsigned char *newB = process_channel(resized_B, newWidth, newHeight, radius, center, mode, position);

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

