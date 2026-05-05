#include "fft_q15.h"
#include "fft_twiddle.h"
#include <stddef.h>

fft_status_t fft_q15_bit_reversal(complex_q15_t *pData, uint32_t n) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    uint32_t j = 0;
    for (uint32_t i = 0; i < n; i++) {
        if (i < j) {
            complex_q15_t temp = pData[i];
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

fft_status_t fft_q15_radix2(complex_q15_t *pData, uint32_t n) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    /* 1. Bit reversal */
    fft_q15_bit_reversal(pData, n);
    
    /* 2. Butterfly computation */
    for (uint32_t len = 2; len <= n; len <<= 1) {
        uint32_t step = MAX_FFT_SIZE / len;
        
        for (uint32_t i = 0; i < n; i += len) {
            for (uint32_t j = 0; j < len / 2; j++) {
                uint32_t idx_u = i + j;
                uint32_t idx_v = i + j + len / 2;
                
                uint32_t lut_idx = j * step;
                q15_t cos_w = twiddle_cos_q15[lut_idx];
                q15_t sin_w = -twiddle_sin_q15[lut_idx]; // Forward FFT
                
                complex_q15_t u = pData[idx_u];
                complex_q15_t v = pData[idx_v];
                
                /* v_prime = v * W */
                /* Q15 * Q15 -> Q30, then shift right by 15 to get Q15 */
                int32_t v_re = ((int32_t)v.real * cos_w - (int32_t)v.imag * sin_w) >> 15;
                int32_t v_im = ((int32_t)v.real * sin_w + (int32_t)v.imag * cos_w) >> 15;
                
                /* Butterfly with scaling by 2 to prevent overflow */
                pData[idx_u].real = (q15_t)(( (int32_t)u.real + v_re ) >> 1);
                pData[idx_u].imag = (q15_t)(( (int32_t)u.imag + v_im ) >> 1);
                pData[idx_v].real = (q15_t)(( (int32_t)u.real - v_re ) >> 1);
                pData[idx_v].imag = (q15_t)(( (int32_t)u.imag - v_im ) >> 1);
            }
        }
    }
    
    return FFT_SUCCESS;
}
