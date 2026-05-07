#include "fft_f32.h"
#include "fft_twiddle.h"
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

fft_status_t fft_f32_bit_reversal(complex_f32_t *pData, uint32_t n) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    uint32_t j = 0;
    for (uint32_t i = 0; i < n; i++) {
        if (i < j) {
            complex_f32_t temp = pData[i];
            pData[i] = pData[j];
            pData[j] = temp;
        }
        uint32_t m = n >> 1;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    return FFT_SUCCESS;
}

fft_status_t fft_f32_radix2_compute(complex_f32_t *pData, uint32_t n, uint8_t isInverse) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;

    /* 2. Butterfly computation */
    for (uint32_t len = 2; len <= n; len <<= 1) {
        uint32_t step = MAX_FFT_SIZE / len;
        
        for (uint32_t i = 0; i < n; i += len) {
            for (uint32_t j = 0; j < len / 2; j++) {
                uint32_t idx_u = i + j;
                uint32_t idx_v = i + j + len / 2;
                
                uint32_t lut_idx = j * step;
                float32_t cos_w = twiddle_cos_f32[lut_idx];
                float32_t sin_w = twiddle_sin_f32[lut_idx];
                
                if (!isInverse) {
                    sin_w = -sin_w;
                }
                
                complex_f32_t u = pData[idx_u];
                complex_f32_t v = pData[idx_v];
                
                /* v_prime = v * W */
                complex_f32_t v_prime;
                v_prime.real = v.real * cos_w - v.imag * sin_w;
                v_prime.imag = v.real * sin_w + v.imag * cos_w;
                
                pData[idx_u].real = u.real + v_prime.real;
                pData[idx_u].imag = u.imag + v_prime.imag;
                pData[idx_v].real = u.real - v_prime.real;
                pData[idx_v].imag = u.imag - v_prime.imag;
            }
        }
    }
    
    /* 3. Scaling for Inverse FFT */
    if (isInverse) {
        float32_t scale = 1.0f / (float32_t)n;
        for (uint32_t i = 0; i < n; i++) {
            pData[i].real *= scale;
            pData[i].imag *= scale;
        }
    }
    
    return FFT_SUCCESS;
}

fft_status_t fft_f32_radix2(complex_f32_t *pData, uint32_t n, uint8_t isInverse) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    /* 1. Bit reversal */
    fft_f32_bit_reversal(pData, n);
    
    /* 2. Butterfly computation */
    return fft_f32_radix2_compute(pData, n, isInverse);
}
