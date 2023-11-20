#include <stdio.h>
#include <imago2.h>
#include <stdlib.h>

#include "utils.h"

int main(int argc, char* argv[]) {
    if(argc != 5){
        fprintf(stderr, 
            "Bad Arguments Error\n "
            "The command must look like :\n"
            "./resize input_file output_file new_width new_height\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments
    char* input_file = argv[1];
    char* output = argv[2];
    int new_width = atoi(argv[3]);
    int new_height = atoi(argv[4]);

    // Open input image
    struct img_pixmap* img = open_img_from_name(input_file);
    int width = img->width;
    int height = img->height;

    // Decompose image to color channels (R, G, B)
    unsigned char* R = channel_extract(img->pixels, width * height, 0);
    unsigned char* G = channel_extract(img->pixels, width * height, 1);
    unsigned char* B = channel_extract(img->pixels, width * height, 2);

    unsigned char* resized_R = resize(R, width, height, new_width, new_height);
    unsigned char* resized_G = resize(G, width, height, new_width, new_height);
    unsigned char* resized_B = resize(B, width, height, new_width, new_height);

    // Recompose image from modified color channels (R, G, B)
    unsigned char* resized_image = channel_compose(resized_R, resized_G, resized_B, new_width * new_height);
    img_save_pixels(output, resized_image, new_width, new_height, IMG_FMT_RGB24);

    free(R);
    free(G);
    free(B);
    free(resized_R);
    free(resized_G);
    free(resized_B);
    free(resized_image);

    exit(EXIT_SUCCESS);
}
