#ifndef FFT_PIPELINE_H
#define FFT_PIPELINE_H

#include "fft_f32.h"

/**
 * @brief Processes a raw DMA buffer into a complex FFT buffer.
 * 
 * This function performs:
 * 1. Conversion from uint16_t to float32_t.
 * 2. DC offset removal (centering).
 * 3. Out-of-place Bit-Reversal (Shuffling).
 * 4. FFT computation.
 * 
 * @param pSrc Pointer to the raw uint16_t DMA buffer (size n).
 * @param pDst Pointer to the destination complex_f32_t buffer (size n).
 * @param n Length of the FFT (must be a power of 2).
 * @param offset DC offset to subtract from raw samples (e.g., 2048.0 for 12-bit ADC).
 * @return fft_status_t 
 */
fft_status_t fft_pipeline_process_f32(const uint16_t *pSrc, complex_f32_t *pDst, uint32_t n, float32_t offset);

#endif /* FFT_PIPELINE_H */
