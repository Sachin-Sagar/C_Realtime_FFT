#include "fft_window.h"
#include <math.h>
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

fft_status_t fft_window_f32(float32_t *pData, uint32_t n, fft_window_type_t type) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    for (uint32_t i = 0; i < n; i++) {
        float32_t w = 0.0f;
        float32_t arg = 2.0f * (float32_t)M_PI * (float32_t)i / (float32_t)(n - 1);
        
        switch (type) {
            case WINDOW_HANN:
                w = 0.5f * (1.0f - cosf(arg));
                break;
            case WINDOW_HAMMING:
                w = 0.54f - 0.46f * cosf(arg);
                break;
            case WINDOW_BLACKMAN:
                w = 0.42f - 0.5f * cosf(arg) + 0.08f * cosf(2.0f * arg);
                break;
        }
        pData[i] *= w;
    }
    return FFT_SUCCESS;
}

fft_status_t fft_window_q15(q15_t *pData, uint32_t n, fft_window_type_t type) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;
    
    for (uint32_t i = 0; i < n; i++) {
        float32_t w = 0.0f;
        float32_t arg = 2.0f * (float32_t)M_PI * (float32_t)i / (float32_t)(n - 1);
        
        switch (type) {
            case WINDOW_HANN:
                w = 0.5f * (1.0f - cosf(arg));
                break;
            case WINDOW_HAMMING:
                w = 0.54f - 0.46f * cosf(arg);
                break;
            case WINDOW_BLACKMAN:
                w = 0.42f - 0.5f * cosf(arg) + 0.08f * cosf(2.0f * arg);
                break;
        }
        /* Q15 multiplication */
        pData[i] = (q15_t)(((int32_t)pData[i] * (int32_t)(w * 32767.0f)) >> 15);
    }
    return FFT_SUCCESS;
}
