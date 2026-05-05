#include "fft_q31.h"
#include "fft_twiddle.h"
#include <stddef.h>

fft_status_t fft_q31_bit_reversal(complex_q31_t *pData, uint32_t n) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    uint32_t j = 0;
    for (uint32_t i = 0; i < n; i++) {
        if (i < j) {
            complex_q31_t temp = pData[i];
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

fft_status_t fft_q31_radix2(complex_q31_t *pData, uint32_t n) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    /* 1. Bit reversal */
    fft_q31_bit_reversal(pData, n);
    
    /* 2. Butterfly computation */
    for (uint32_t len = 2; len <= n; len <<= 1) {
        uint32_t step = MAX_FFT_SIZE / len;
        
        for (uint32_t i = 0; i < n; i += len) {
            for (uint32_t j = 0; j < len / 2; j++) {
                uint32_t idx_u = i + j;
                uint32_t idx_v = i + j + len / 2;
                
                uint32_t lut_idx = j * step;
                q31_t cos_w = twiddle_cos_q31[lut_idx];
                q31_t sin_w = -twiddle_sin_q31[lut_idx]; // Forward FFT
                
                complex_q31_t u = pData[idx_u];
                complex_q31_t v = pData[idx_v];
                
                /* v_prime = v * W */
                /* Q31 * Q31 -> Q62, then shift right by 31 to get Q31 */
                int64_t v_re = ((int64_t)v.real * cos_w - (int64_t)v.imag * sin_w) >> 31;
                int64_t v_im = ((int64_t)v.real * sin_w + (int64_t)v.imag * cos_w) >> 31;
                
                /* Butterfly with scaling by 2 to prevent overflow */
                pData[idx_u].real = (q31_t)(( (int64_t)u.real + v_re ) >> 1);
                pData[idx_u].imag = (q31_t)(( (int64_t)u.imag + v_im ) >> 1);
                pData[idx_v].real = (q31_t)(( (int64_t)u.real - v_re ) >> 1);
                pData[idx_v].imag = (q31_t)(( (int64_t)u.imag - v_im ) >> 1);
            }
        }
    }
    
    return FFT_SUCCESS;
}
