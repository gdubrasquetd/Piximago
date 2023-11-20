#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <complex.h>
#include <fftw3.h>
#include <imago2.h>
#include <string.h>

static char* RGB[3] = {"R-","G-","B-"};
static int RGBcpt = 0;

static char* RESULT_PATH = "";
static char* SPECTRUM_PATH = "./tmp/spectrum/";
static char* MASK_PATH = "./tmp/mask/";
static char* BACK_PATH = "../../";


struct img_pixmap *open_img_from_name(char *file_name);

unsigned char *channel_extract(unsigned char *pixels, int size, int channel_number);

unsigned char *channel_compose(unsigned char *r_chan, unsigned char *g_chan, unsigned char *b_chan, size_t size);

void save_image_double(double *spectrum, int width, int height, char* dir, char *name);

void erosion(double *img, int width, int height, int radius);

void dilatation(double *img, int width, int height, int radius);

char* concat_char(char *str1, char *str2);

void img_convert_double_to_uchar(double* img1, unsigned char* img2, int width, int height);

double normalized_value_on_spectrum(double* img, int width, int height, int threshold);

int threshold_from_histogram_cumulate(unsigned char* img, int width, int height, int percent);

unsigned char check_rgb_range(double value);

unsigned char* resize(unsigned char* channel, int width, int height, int newWidth, int newHeight);

unsigned char* normalizeImageDimension(unsigned char* channel, int width, int height, int *new_width, int * new_height, int max_size);


#endif 
