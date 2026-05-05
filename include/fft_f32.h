#ifndef FFT_F32_H
#define FFT_F32_H

#include "fft_config.h"

/**
 * @brief Performs in-place bit-reversal of a complex f32 array.
 * 
 * @param pData Pointer to the complex data array.
 * @param n Length of the array (must be a power of 2).
 * @return fft_status_t 
 */
fft_status_t fft_f32_bit_reversal(complex_f32_t *pData, uint32_t n);

/**
 * @brief Performs in-place Radix-2 FFT on a complex f32 array.
 * 
 * @param pData Pointer to the complex data array.
 * @param n Length of the FFT (must be a power of 2).
 * @param isInverse 0 for forward FFT, 1 for inverse FFT.
 * @return fft_status_t 
 */
fft_status_t fft_f32_radix2(complex_f32_t *pData, uint32_t n, uint8_t isInverse);

#endif /* FFT_F32_H */
