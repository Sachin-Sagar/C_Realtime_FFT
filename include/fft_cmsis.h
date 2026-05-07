#ifndef FFT_CMSIS_H
#define FFT_CMSIS_H

#include "fft_config.h"

/* 
 * NOTE: This requires the ARM CMSIS-DSP library to be linked.
 * It will not compile on x86/Linux without an ARM toolchain.
 */

#ifdef USE_CMSIS_DSP
#include "arm_math.h"

/**
 * @brief Performs a high-performance FFT using ARM CMSIS-DSP.
 * 
 * @param pData Pointer to the complex data array (interleaved real/imag).
 * @param n Length of the FFT (must be a power of 2).
 * @param isInverse 0 for forward FFT, 1 for inverse FFT.
 * @return fft_status_t 
 */
fft_status_t fft_cmsis_f32(complex_f32_t *pData, uint32_t n, uint8_t isInverse);

#endif /* USE_CMSIS_DSP */

#endif /* FFT_CMSIS_H */
