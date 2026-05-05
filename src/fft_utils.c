#include "fft_utils.h"
#include <math.h>
#include <stddef.h>

fft_status_t fft_f32_magnitude(const complex_f32_t *pSrc, float32_t *pDst, uint32_t n) {
    if (pSrc == NULL || pDst == NULL) return FFT_ERROR_NULL_POINTER;
    
    for (uint32_t i = 0; i < n; i++) {
        pDst[i] = sqrtf(pSrc[i].real * pSrc[i].real + pSrc[i].imag * pSrc[i].imag);
    }
    
    return FFT_SUCCESS;
}

fft_status_t fft_q15_magnitude(const complex_q15_t *pSrc, q15_t *pDst, uint32_t n) {
    if (pSrc == NULL || pDst == NULL) return FFT_ERROR_NULL_POINTER;
    
    for (uint32_t i = 0; i < n; i++) {
        int32_t re = pSrc[i].real;
        int32_t im = pSrc[i].imag;
        /* Q15*Q15 + Q15*Q15 -> Q30 sum */
        int32_t sum_sq = re * re + im * im;
        /* sqrt of Q30 is Q15 */
        pDst[i] = (q15_t)sqrt(sum_sq);
    }
    
    return FFT_SUCCESS;
}

fft_status_t fft_q31_magnitude(const complex_q31_t *pSrc, q31_t *pDst, uint32_t n) {
    if (pSrc == NULL || pDst == NULL) return FFT_ERROR_NULL_POINTER;
    
    for (uint32_t i = 0; i < n; i++) {
        int64_t re = pSrc[i].real;
        int64_t im = pSrc[i].imag;
        /* Q31*Q31 + Q31*Q31 -> Q62 sum */
        int64_t sum_sq = re * re + im * im;
        /* sqrt of Q62 is Q31 */
        pDst[i] = (q31_t)sqrt(sum_sq);
    }
    
    return FFT_SUCCESS;
}
