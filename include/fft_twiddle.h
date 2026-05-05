#ifndef FFT_TWIDDLE_H
#define FFT_TWIDDLE_H

#include "fft_config.h"

#define MAX_FFT_SIZE 4096

/* Pre-computed twiddle factors (sin/cos tables) */
extern float32_t twiddle_sin_f32[MAX_FFT_SIZE];
extern float32_t twiddle_cos_f32[MAX_FFT_SIZE];

extern q15_t twiddle_sin_q15[MAX_FFT_SIZE];
extern q15_t twiddle_cos_q15[MAX_FFT_SIZE];

extern q31_t twiddle_sin_q31[MAX_FFT_SIZE];
extern q31_t twiddle_cos_q31[MAX_FFT_SIZE];

void fft_twiddle_init_f32(void);
void fft_twiddle_init_q15(void);
void fft_twiddle_init_q31(void);

#endif /* FFT_TWIDDLE_H */
