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

// gcc stain.c fft.c utils.c -limago -lfftw3 -lpng -ljpeg -lm -ldl -o ../build/stain
// ./stain input output R_threshold G_threshold B_threshold radius 

// Delete mask central stain
void delete_center_stain(double *img, int width, int height){

    double marked = 0.2;

    // Marked a cross of 5 pixels in the center of the image
    img[(height/2-1) * width + width/2-1] = marked;
    img[(height/2-1) * width + width/2+1] = marked;
    img[(height/2+1) * width + width/2-1] = marked;
    img[(height/2+1) * width + width/2+1] = marked;
    img[(height/2) * width + width/2] = marked;

    // Divide the image in 4 parts to browse the image from the middle
    int i1, i2, i3, i4, j1, j2, j3, j4; 
    for(i1 = height/2+1, i2 = height/2+1, i3 = height/2-1, i4=height/2-1 ; i1 > 0; i1--, i2--, i3++, i4++) {
        for(j1 = width/2+1, j2 = width/2+1, j3 = width/2+1, j4=width/2-1 ; j1 > 0; j1--, j2++, j3--, j4++) {
            int ind1 = i1*width + j1;
            int ind2 = i2*width + j2;
            int ind3 = i3*width + j3;
            int ind4 = i4*width + j4;

            // Marked pixel if there is one of its neighbors is already marked
            if(img[ind1] == 0.0 && (img[ind1-1] == marked || img[ind1+1] == marked || img[ind1-width] == marked  || img[ind1-width-1] == marked  || img[ind1-width+1] == marked || img[ind1+width] == marked || img[ind1+width-1] == marked || img[ind1+width+1] == marked)){
                img[ind1] = marked;
            }
            if(img[ind2] == 0.0 && (img[ind2-1] == marked || img[ind2+1] == marked || img[ind2-width] == marked  || img[ind2-width-1] == marked  || img[ind2-width+1] == marked || img[ind2+width] == marked || img[ind2+width-1] == marked || img[ind2+width+1] == marked)){
                img[ind2] = marked;
            }
            if(img[ind3] == 0.0 && (img[ind3-1] == marked || img[ind3+1] == marked || img[ind3-width] == marked  || img[ind3-width-1] == marked  || img[ind3-width+1] == marked || img[ind3+width] == marked || img[ind3+width-1] == marked || img[ind3+width+1] == marked)){
                img[ind3] = marked;
            }
            if(img[ind4] == 0.0 && (img[ind4-1] == marked || img[ind4+1] == marked || img[ind4-width] == marked  || img[ind4-width-1] == marked  || img[ind4-width+1] == marked || img[ind4+width] == marked || img[ind4+width-1] == marked || img[ind4+width+1] == marked)){
                img[ind4] = marked;
            }
        }
    }

    // Same operation in the opposite direction, from the corners to the center
    for(i1 = 0, i2 = 0, i3 = height-1, i4=height-1 ; i1 < height/2+1; i1++, i2++, i3--, i4--) {
        for(j1 = 0, j2 = width-1, j3 = 0, j4=width-1 ; j1 < width/2+1; j1++, j2--, j3++, j4--) {
            int ind1 = i1*width + j1;
            int ind2 = i2*width + j2;
            int ind3 = i3*width + j3;
            int ind4 = i4*width + j4;

            if(img[ind1] == 0.0 && (img[ind1-1] == marked || img[ind1+1] == marked || img[ind1-width] == marked  || img[ind1-width-1] == marked  || img[ind1-width+1] == marked || img[ind1+width] == marked || img[ind1+width-1] == marked || img[ind1+width+1] == marked)){
                img[ind1] = marked;
            }
            if(img[ind2] == 0.0 && (img[ind2-1] == marked || img[ind2+1] == marked || img[ind2-width] == marked  || img[ind2-width-1] == marked  || img[ind2-width+1] == marked || img[ind2+width] == marked || img[ind2+width-1] == marked || img[ind2+width+1] == marked)){
                img[ind2] = marked;
            }
            if(img[ind3] == 0.0 && (img[ind3-1] == marked || img[ind3+1] == marked || img[ind3-width] == marked  || img[ind3-width-1] == marked  || img[ind3-width+1] == marked || img[ind3+width] == marked || img[ind3+width-1] == marked || img[ind3+width+1] == marked)){
                img[ind3] = marked;
            }
            if(img[ind4] == 0.0 && (img[ind4-1] == marked || img[ind4+1] == marked || img[ind4-width] == marked  || img[ind4-width-1] == marked  || img[ind4-width+1] == marked || img[ind4+width] == marked || img[ind4+width-1] == marked || img[ind4+width+1] == marked)){
                img[ind4] = marked;
            }
        }
    }
}

void cut_adhoc(double *spectrum, int width, int height, float threshold, int radius) {
    double *mask = (double *)malloc(width * height * sizeof(double));

    int value = 0;
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
            value += spectrum[ind];

            if(spectrum[ind] > threshold ){ 
                mask[ind] = 0.0;
            }else{
                mask[ind] = 255.0;
            }
        }
    }
    
    dilatation(mask, width, height, radius);
    erosion(mask, width, height, radius);

    delete_center_stain(mask, width, height);

    // Save modified each channel mask to png
    save_image_double(mask, width, height, MASK_PATH, concat_char(RGB[RGBcpt], "mask.png"));

    for (int i = 0; i < width * height; i++) {
        if(mask[i] == 0.0){
            spectrum[i] = mask[i];
        }
    }

    free(mask);
}



static unsigned char* process_channel(unsigned char *channel, int width, int height, float percent, int radius)
{
    // Extracting spectrum using Fourier transform
    fftw_complex *fr_channel = fft_forward(channel, width, height);
    double *as = fft_extract_as(fr_channel, width, height);
    double *ps = fft_extract_ps(fr_channel, width, height);

    int threshold_int = threshold_from_histogram_cumulate(channel, width, height, percent);

    double threshold = normalized_value_on_spectrum(as, width, height, threshold_int);

    cut_adhoc(as, width, height, threshold, radius);
    
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
            "./stain input_file output_file R_treshold G_treshold B_treshold radius_closing\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments
    char* input_file = argv[1];
    char* output_file = concat_char(RESULT_PATH, argv[2]);
    float percentR = atof(argv[3]);
    float percentG = atof(argv[4]);
    float percentB = atof(argv[5]);
    int radius = atoi(argv[6]);

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
    unsigned char *resized_R = normalizeImageDimension(R, width, height, &newWidth, &newHeight, 600);
    unsigned char *resized_G = normalizeImageDimension(G, width, height, &newWidth, &newHeight, 600);
    unsigned char *resized_B = normalizeImageDimension(B, width, height, &newWidth, &newHeight, 600);

    unsigned char *newR = process_channel(resized_R, newWidth, newHeight, percentR, radius);
    unsigned char *newG = process_channel(resized_G, newWidth, newHeight, percentG, radius);
    unsigned char *newB = process_channel(resized_B, newWidth, newHeight, percentB, radius);

    // Recompose image from modified color channels (R, G, B)
    unsigned char *res = channel_compose(newR, newG, newB, newWidth * newHeight);
    img_save_pixels(output_file, res, newWidth, newHeight, IMG_FMT_RGB24);

    free(resized_R);
    free(resized_G);
    free(resized_B);
    free(newR);
    free(newG);
    free(newB);
    free(res);

    exit(EXIT_SUCCESS);
}

