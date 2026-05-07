#include "fft_cmsis.h"
#include <stddef.h>

#ifdef USE_CMSIS_DSP

fft_status_t fft_cmsis_f32(complex_f32_t *pData, uint32_t n, uint8_t isInverse) {
    if (pData == NULL) return FFT_ERROR_NULL_POINTER;

    /* CMSIS-DSP uses pre-defined instance structures for different sizes */
    const arm_cfft_instance_f32 *S;

    switch (n) {
        case 64:   S = &arm_cfft_sR_f32_len64;   break;
        case 128:  S = &arm_cfft_sR_f32_len128;  break;
        case 256:  S = &arm_cfft_sR_f32_len256;  break;
        case 512:  S = &arm_cfft_sR_f32_len512;  break;
        case 1024: S = &arm_cfft_sR_f32_len1024; break;
        case 2048: S = &arm_cfft_sR_f32_len2048; break;
        case 4096: S = &arm_cfft_sR_f32_len4096; break;
        default: return FFT_ERROR_INVALID_LENGTH;
    }

    /* 
     * CMSIS-DSP arm_cfft_f32 parameters:
     * 1. S: Pointer to the cfft instance
     * 2. p1: Pointer to the data (interleaved real/imag)
     * 3. ifftFlag: 0 for forward, 1 for inverse
     * 4. bitReverseFlag: 1 for enabled (always use 1 for standard FFT)
     */
    arm_cfft_f32(S, (float32_t *)pData, isInverse, 1);

    return FFT_SUCCESS;
}

#endif /* USE_CMSIS_DSP */
