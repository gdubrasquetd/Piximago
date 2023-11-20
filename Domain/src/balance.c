#include <stdio.h>
#include <imago2.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

//gcc balance.c utils.c -limago -lm -lpng -ljpeg -o ../build/balance
// relevant values :  0 < contrast < 1000    /  default value : 100
// relevant values :  -255 < brightness < 255   /  default value : 0

 int main(int argc, char* argv[]){

    if(argc != 5){
        fprintf(stderr, 
            "Bad Arguments Error\n "
            "The command must look like :\n"
            "./balance input_file output_file contrast brightness\n");
        exit(EXIT_FAILURE);
    }

    // Extract command line arguments
    char *input_file = argv[1];
    char *output = argv[2];
    float factor = atof(argv[3]) / 100;
    float delta = atof(argv[4]);


    // Open input image
    struct img_pixmap *img = open_img_from_name(input_file);
    int width = img->width;
    int height = img->height;

    // Decompose image to color channels (R, G, B)
	unsigned char *R = channel_extract(img->pixels, width * height, 0);
	unsigned char *G = channel_extract(img->pixels, width * height, 1);
	unsigned char* B = channel_extract(img->pixels, width * height, 2);


    for(int i = 0; i < width * height; i++){

        double average = (R[i] + G[i] + B[i])/3.0;

        R[i] = check_rgb_range(((R[i] - average) * factor + average) + delta);
        G[i] = check_rgb_range(((G[i] - average) * factor + average) + delta);
        B[i] = check_rgb_range(((B[i] - average) * factor + average) + delta);

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
