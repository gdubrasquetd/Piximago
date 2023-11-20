#include <stdio.h>
#include <imago2.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

//gcc colors.c utils.c -limago -lm -lpng -ljpeg -o colors
// relevant values : 0 < R,G,B < 300 / default value : 100

 int main(int argc, char* argv[]){

    if(argc != 6){
        fprintf(stderr, 
            "Bad Arguments Error\n "
            "The command must look like :\n"
            "./colors input_file output_file R_percent G_percent B_percent\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments    
    char *input_file = argv[1];
    char *output = argv[2];
    float newR = atof(argv[3]) / 100;
    float newG = atof(argv[4]) / 100;
    float newB = atof(argv[5]) / 100;

    // Open input image
    struct img_pixmap *img = open_img_from_name(input_file);
    int width = img->width;
    int height = img->height;

    // Decompose image to color channels (R, G, B)
	unsigned char *R = channel_extract(img->pixels, width * height, 0);
	unsigned char *G = channel_extract(img->pixels, width * height, 1);
	unsigned char* B = channel_extract(img->pixels, width * height, 2);

    for(int i = 0; i < width * height; i++){
        R[i] = check_rgb_range(R[i] * newR);
        G[i] = check_rgb_range(G[i] * newG);
        B[i] = check_rgb_range(B[i] * newB);
    }

    // Recompose image from modified color channels (R, G, B)
    unsigned char *res = channel_compose(R, G, B, width * height);
    img_save_pixels(output, res, width, height, IMG_FMT_RGB24);

    free(R);
    free(G);
    free(B);
	free(res);

    exit(EXIT_SUCCESS);
}
