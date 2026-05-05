#ifndef FFT_CONFIG_H
#define FFT_CONFIG_H

#include <stdint.h>

/* FFT Types */
typedef float float32_t;
typedef int16_t q15_t;
typedef int32_t q31_t;

/* Complex number structures */
typedef struct {
    float32_t real;
    float32_t imag;
} complex_f32_t;

typedef struct {
    q15_t real;
    q15_t imag;
} complex_q15_t;

typedef struct {
    q31_t real;
    q31_t imag;
} complex_q31_t;

/* FFT Status codes */
typedef enum {
    FFT_SUCCESS = 0,
    FFT_ERROR_INVALID_LENGTH,
    FFT_ERROR_NULL_POINTER
} fft_status_t;

#endif /* FFT_CONFIG_H */
