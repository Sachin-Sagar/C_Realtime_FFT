#ifndef FFT_Q31_H
#define FFT_Q31_H

#include "fft_config.h"

/**
 * @brief Performs in-place bit-reversal of a complex q31 array.
 * 
 * @param pData Pointer to the complex data array.
 * @param n Length of the array.
 * @return fft_status_t 
 */
fft_status_t fft_q31_bit_reversal(complex_q31_t *pData, uint32_t n);

/**
 * @brief Performs in-place Radix-2 FFT on a complex q31 array.
 * This implementation includes scaling at each stage to prevent overflow.
 * 
 * @param pData Pointer to the complex data array.
 * @param n Length of the FFT.
 * @return fft_status_t 
 */
fft_status_t fft_q31_radix2(complex_q31_t *pData, uint32_t n);

#endif /* FFT_Q31_H */
