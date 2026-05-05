#ifndef FFT_Q15_H
#define FFT_Q15_H

#include "fft_config.h"

/**
 * @brief Performs in-place bit-reversal of a complex q15 array.
 * 
 * @param pData Pointer to the complex data array.
 * @param n Length of the array.
 * @return fft_status_t 
 */
fft_status_t fft_q15_bit_reversal(complex_q15_t *pData, uint32_t n);

/**
 * @brief Performs in-place Radix-2 FFT on a complex q15 array.
 * This implementation includes scaling at each stage to prevent overflow.
 * 
 * @param pData Pointer to the complex data array.
 * @param n Length of the FFT.
 * @return fft_status_t 
 */
fft_status_t fft_q15_radix2(complex_q15_t *pData, uint32_t n);

#endif /* FFT_Q15_H */
