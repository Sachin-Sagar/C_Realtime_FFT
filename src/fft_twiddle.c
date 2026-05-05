#include "fft_twiddle.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float32_t twiddle_sin_f32[MAX_FFT_SIZE];
float32_t twiddle_cos_f32[MAX_FFT_SIZE];

q15_t twiddle_sin_q15[MAX_FFT_SIZE];
q15_t twiddle_cos_q15[MAX_FFT_SIZE];

q31_t twiddle_sin_q31[MAX_FFT_SIZE];
q31_t twiddle_cos_q31[MAX_FFT_SIZE];

void fft_twiddle_init_f32(void) {
    for (int i = 0; i < MAX_FFT_SIZE; i++) {
        twiddle_sin_f32[i] = (float32_t)sin(2.0 * M_PI * i / MAX_FFT_SIZE);
        twiddle_cos_f32[i] = (float32_t)cos(2.0 * M_PI * i / MAX_FFT_SIZE);
    }
}

void fft_twiddle_init_q15(void) {
    for (int i = 0; i < MAX_FFT_SIZE; i++) {
        twiddle_sin_q15[i] = (q15_t)floor(32767.5 * sin(2.0 * M_PI * i / MAX_FFT_SIZE));
        twiddle_cos_q15[i] = (q15_t)floor(32767.5 * cos(2.0 * M_PI * i / MAX_FFT_SIZE));
    }
}

void fft_twiddle_init_q31(void) {
    for (int i = 0; i < MAX_FFT_SIZE; i++) {
        twiddle_sin_q31[i] = (q31_t)floor(2147483647.5 * sin(2.0 * M_PI * i / MAX_FFT_SIZE));
        twiddle_cos_q31[i] = (q31_t)floor(2147483647.5 * cos(2.0 * M_PI * i / MAX_FFT_SIZE));
    }
}
