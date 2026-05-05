#ifndef FFT_UTILS_H
#define FFT_UTILS_H

#include "fft_config.h"

/**
 * @brief Computes the magnitude of a complex f32 array.
 * 
 * @param pSrc Pointer to the complex source array.
 * @param pDst Pointer to the destination magnitude array (real f32).
 * @param n Length of the array.
 * @return fft_status_t 
 */
fft_status_t fft_f32_magnitude(const complex_f32_t *pSrc, float32_t *pDst, uint32_t n);
fft_status_t fft_q15_magnitude(const complex_q15_t *pSrc, q15_t *pDst, uint32_t n);
fft_status_t fft_q31_magnitude(const complex_q31_t *pSrc, q31_t *pDst, uint32_t n);

#endif /* FFT_UTILS_H */
