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


// Open an image from a file and convert it to RGB format if needed
struct img_pixmap *open_img_from_name(char *file_name){
    
    struct img_pixmap *img = img_create();

    if (img_load(img, file_name) == -1)
    {
	fprintf(stderr, "Cannot load \"%s\" image.\n Please check the name and the location of the image.", file_name);
	exit(EXIT_FAILURE);
    }

    if (img_is_float(img) || img_has_alpha(img)|| img_is_greyscale(img))
    {
    	img_convert(img, IMG_FMT_RGB24);
    }
    return img;
}


// Extract a specific color channel from an RGB image
unsigned char *channel_extract(unsigned char *pixels, int size, int channel_number){
    unsigned char *channel_pixels = (unsigned char*)malloc(size * sizeof(char));
    int ch = 0;
    for(int i=0; i < size * sizeof(char); i++){
        channel_pixels[i] = pixels[ch + channel_number];
        ch += 3;
    }

    return channel_pixels;
}

// Compose three color channels (R, G, B) into a single RGB image
unsigned char *channel_compose(unsigned char *r_chan, unsigned char *g_chan, unsigned char *b_chan, size_t size){
    int img_buffer_size = size * 3 * sizeof(char);
    unsigned char *composed_img = (unsigned char*)malloc(img_buffer_size);
    int channel = 0;
    for(int i=0; channel < img_buffer_size; i++){
        composed_img[channel] = r_chan[i];
        composed_img[channel+1] = g_chan[i];
        composed_img[channel+2] = b_chan[i];
        channel += 3;
    }

    return composed_img;
}

// Save a double spectrum image to a file after converting it to uchar format
void save_image_double(double *spectrum, int width, int height, char* dir, char *name) {

    if (!chdir(dir) == 0) 
        fprintf(stderr, "Directory error");

    unsigned char *pix = calloc(width * height, sizeof(unsigned char));

    img_convert_double_to_uchar(spectrum, pix, width, height);

    struct img_pixmap *img = img_create();
    img_set_pixels(img, width, height, IMG_FMT_GREY8, pix);
    img_save(img, name);

    img_free(img);
    free(pix);

    if (!chdir(BACK_PATH) == 0) 
        fprintf(stderr, "Directory error");

}

// Apply erosion operation to an image
void erosion(double *img, int width, int height, int radius) {
    double *tmp_image = (double *)malloc(width * height * sizeof(double));

    if (tmp_image == NULL) {
        fprintf(stderr, "Memory allocation error for temporary image.\n");
        exit(EXIT_FAILURE);
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < radius; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = height-radius; y < height; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = 0; x < radius; x++) {
        for (int y = 0; y < height; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = width-radius; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = radius; x < width - radius; x++) {
        for (int y = radius; y < height - radius; y++) {
            double min_val = 1.0;

            for (int i = -radius; i <= radius; i++) {
                for (int j = -radius; j <= radius; j++) {
                    if (i * i + j * j <= radius * radius) {
                        int nx = x + i;
                        int ny = y + j;

                        double pixel_val = img[ny * width + nx];
                        if (pixel_val < min_val) {
                            min_val = pixel_val;
                        }
                    }
                }
            }
            tmp_image[y * width + x] = min_val;
        }
    }

    for (int i = 0; i < width * height; i++) {
        img[i] = tmp_image[i];
    }

    free(tmp_image);
}


// Apply dilatation operation to an image
void dilatation(double *img, int width, int height, int radius) {
    double *tmp_image = (double *)malloc(width * height * sizeof(double));

    if (tmp_image == NULL) {
        fprintf(stderr, "Memory allocation error for temporary image.\n");
        exit(EXIT_FAILURE);
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < radius; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = height-radius; y < height; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = 0; x < radius; x++) {
        for (int y = 0; y < height; y++) {
            tmp_image[y * width + x] = 255;
        }
    }

    for (int x = width-radius; x < width; x++) {
        for (int y = 0; y < height; y++) {
            tmp_image[y * width + x] = 255;
        }
    }


    for (int x = radius; x < width - radius; x++) {
        for (int y = radius; y < height - radius; y++) {
            double max_val = 0.0;

            for (int i = -radius; i <= radius; i++) {
                for (int j = -radius; j <= radius; j++) {
                    if (i * i + j * j <= radius * radius) {
                        int nx = x + i;
                        int ny = y + j;

                        double pixel_val = img[ny * width + nx];
                        if (pixel_val > max_val) {
                            max_val = pixel_val;
                        }
                    }
                }
            }

            tmp_image[y * width + x] = max_val;
        }
    }

    for (int i = 0; i < width * height; i++) {
        img[i] = tmp_image[i];
    }

    free(tmp_image);
}

// Concatenate two strings
char* concat_char(char *str1, char *str2) {
    char *result = (char *)malloc(strlen(str1) + strlen(str2) + 1);
    if (result == NULL) {
        fprintf(stderr, "Error in file's name.\n");
        exit(1);
    }
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

// Convert image using double values to image using uchar values
void img_convert_double_to_uchar(double* img1, unsigned char* img2, int width, int height){
    
    double max = img1[0];
    for(int i = 1; i < width * height; i++) {
        if(max < img1[i]) {
            max = img1[i];
        }
    }

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
            img2[ind] = (unsigned char) (pow((img1[ind]/max), 0.15) * 255);
        }
    }
}

// Calculate the normalized value on the spectrum based on a threshold
double normalized_value_on_spectrum(double* img, int width, int height, int threshold){
    
    double max = img[0];
    for(int i = 1; i < width * height; i++) {
        if(max < img[i]) {
            max = img[i];
        }
    }

    double result = pow((threshold / 255.0), 1.0 / 0.15)*max;
    return result;

}

// Calculate the threshold value from the cumulative histogram of an image
int threshold_from_histogram_cumulate(unsigned char* img, int width, int height, int percent){
    int *hist = (int*)calloc(256, sizeof(int));
    for(int i=0; i < width * height; i++){
        hist[img[i]] += 1;
    }
    for(int i=255; i >= 0; i--){
        for(int j=0; j < i; j++){
            hist[i] += hist[j];
        }

        if(hist[i] < (width * height) * percent /100){
            return i;
        }
    }
    free(hist);
    return 0;
}

// Check and limit the value of a color channel to the valid range [0, 255]
unsigned char check_rgb_range(double value){
    if(value >= 255.0){
        return 255;
    }else {
        if(value <= 0.0){
            return 0;
        }else{
            return (unsigned char) value;
        }
    }
}

// Resize a channel using bilinear interpolation
unsigned char* resize(unsigned char* channel, int width, int height, int newWidth, int newHeight) {
    // Calculate scaling factors
    double xScale = (double)width / newWidth;
    double yScale = (double)height / newHeight;

    // Allocate memory for the resized channel
    size_t new_size = newWidth * newHeight;
    unsigned char* resizedChannel = (unsigned char*)malloc(new_size);

    if (resizedChannel == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    // Redimension the channel by performing bilinear interpolation
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            double sourceX = x * xScale;
            double sourceY = y * yScale;

            int sourceX1 = (int)sourceX;
            int sourceY1 = (int)sourceY;
            int sourceX2 = sourceX1 + 1;
            int sourceY2 = sourceY1 + 1;

            double alpha = sourceX - sourceX1;
            double beta = sourceY - sourceY1;

            int destIndex = y * newWidth + x;

            resizedChannel[destIndex] = (1 - alpha) * (1 - beta) * channel[sourceY1 * width + sourceX1] +
                                       alpha * (1 - beta) * channel[sourceY1 * width + sourceX2] +
                                       (1 - alpha) * beta * channel[sourceY2 * width + sourceX1] +
                                       alpha * beta * channel[sourceY2 * width + sourceX2];
        }
    }

    return resizedChannel;
}

// Normalize image dimensions to fit within a maximum size
unsigned char* normalizeImageDimension(unsigned char* channel, int width, int height, int *new_width, int * new_height, int max_size){
    int newWidth;
    int newHeight;
    if(width > max_size || height > max_size){
        if(width > height){
            newWidth = max_size;
            newHeight = height * max_size / width;
        }else{
            newWidth = width * max_size / height;
            newHeight = max_size;
        }
        unsigned char* res = resize(channel, width, height, newWidth, newHeight);
        free(channel);
        *new_width = newWidth;
        *new_height = newHeight;
        return res;
    }else{
        *new_width = width;
        *new_height = height;
        return channel;
    }
}