#include "fft_pipeline.h"
#include <stddef.h>

fft_status_t fft_pipeline_process_f32(const uint16_t *pSrc, complex_f32_t *pDst, uint32_t n, float32_t offset) {
    if (pSrc == NULL || pDst == NULL) return FFT_ERROR_NULL_POINTER;

    /* 1. Out-of-place Conversion and Bit-Reversal Shuffle */
    /* We calculate the bit-reversed index j for each input index i */
    uint32_t j = 0;
    for (uint32_t i = 0; i < n; i++) {
        /* Convert uint16_t -> float32_t and remove DC offset */
        pDst[j].real = (float32_t)pSrc[i] - offset;
        pDst[j].imag = 0.0f;

        /* Calculate next bit-reversed index j */
        uint32_t m = n >> 1;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    /* 2. Run the FFT Butterfly stages (skip bit-reversal as it's already done) */
    return fft_f32_radix2_compute(pDst, n, 0);
}
