#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include <complex.h>
#include <fftw3.h>
#include <imago2.h>

#include "fft.h"

/*TO MODIFY*/
static fftw_complex *conv_uchar_to_complex(unsigned char *channel, int width, int height){

    fftw_complex *c = (fftw_complex*) fftw_malloc(width * height * sizeof(fftw_complex));
    float *tab = (float*) malloc(width * height * sizeof(float));

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
            tab[ind] = (float)channel[ind];
            tab[ind] =  tab[ind] * pow(-1, j+i) ;
            c[ind] = (fftw_complex)(tab[ind] / (width * height));
        }
    }
    
    free(tab);
    return c;
}

/*TO MODIFY*/
static unsigned char *conv_complex_to_uchar(fftw_complex *c_channel, int width, int height){

    unsigned char *cha = (unsigned char*) malloc(width * height * sizeof(unsigned char));

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
            float f = (float) pow(-1, j+i) * c_channel[ind];
            if(f > 255.0)
                f = 255.0;
            if(f < 0.0)
                f = 0.0;
            cha[ind] = (unsigned char) f;
        }
    }
    return cha;
}

/*TO MODIFY*/
fftw_complex *fft_forward(unsigned char *chan, int width, int height){

    fftw_complex *in = conv_uchar_to_complex(chan, width, height);
    fftw_complex *out = (fftw_complex*) fftw_malloc(width * height * sizeof(fftw_complex));

    fftw_plan p = fftw_plan_dft_2d(height, width, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    fftw_destroy_plan(p);
    fftw_free(in);
    return out;
}

/*TO MODIFY*/
unsigned char *fft_backward(fftw_complex *fr_chan, int width, int height){
    fftw_complex *out = (fftw_complex*) fftw_malloc(width * height * sizeof(fftw_complex));

    fftw_plan p = fftw_plan_dft_2d(height, width, fr_chan, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    fftw_destroy_plan(p);
    unsigned char *ch = conv_complex_to_uchar(out, width, height);
    fftw_free(out);

    return ch;
}

/*TO MODIFY*/
double *fft_extract_as(fftw_complex *fr_chan, int width, int height){

    double *amp = (double*) malloc(width * height * sizeof(double));

     for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
        	amp[ind] = cabs(fr_chan[ind]);
		}
    }

    return amp;
}

/*TO MODIFY*/
double *fft_extract_ps(fftw_complex *fr_chan, int width, int height){
    double *ph = (double*) malloc(width * height * sizeof(double));
	double re, im;

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            int ind = i*width + j;
            re = creal(fr_chan[ind]);
			im = cimag(fr_chan[ind]);
        	ph[ind] = atan2(im, re);
        }
    }

    return ph;
}

/*TO MODIFY*/
fftw_complex *fft_compose_aps(double *as, double *ps, int width, int height){
    fftw_complex *aps = (fftw_complex*) fftw_malloc(width * height * sizeof(fftw_complex));

    for(int v=0; v < height; v++) {
        for(int u=0; u < width; u++) {
            int ind = v*width + u;
            aps[ind] = as[ind] * cos(ps[ind]) + I*as[ind] * sin(ps[ind]);
        }
    }

    return aps;
}


