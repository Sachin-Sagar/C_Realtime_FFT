#ifndef FFT_WINDOW_H
#define FFT_WINDOW_H

#include "fft_config.h"

typedef enum {
    WINDOW_HANN = 0,
    WINDOW_HAMMING,
    WINDOW_BLACKMAN
} fft_window_type_t;

/**
 * @brief Applies a window function to a real f32 array.
 * 
 * @param pData Pointer to the data array.
 * @param n Length of the array.
 * @param type Window type.
 * @return fft_status_t 
 */
fft_status_t fft_window_f32(float32_t *pData, uint32_t n, fft_window_type_t type);

/**
 * @brief Applies a window function to a q15 array.
 * 
 * @param pData Pointer to the data array.
 * @param n Length of the array.
 * @param type Window type.
 * @return fft_status_t 
 */
fft_status_t fft_window_q15(q15_t *pData, uint32_t n, fft_window_type_t type);

#endif /* FFT_WINDOW_H */
