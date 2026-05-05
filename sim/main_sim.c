#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "fft_f32.h"
#include "fft_q15.h"
#include "fft_q31.h"
#include "fft_twiddle.h"
#include "fft_utils.h"
#include "fft_window.h"

#define SAMPLING_FREQ 1000.0f
#define SIGNAL_FREQ1 50.0f
#define SIGNAL_FREQ2 120.0f
#define FFT_SIZE 1024

int main() {
    complex_f32_t data_f32[FFT_SIZE];
    complex_q15_t data_q15[FFT_SIZE];
    complex_q31_t data_q31[FFT_SIZE];
    
    float32_t mag_f32[FFT_SIZE];
    q15_t mag_q15[FFT_SIZE];
    q31_t mag_q31[FFT_SIZE];
    
    /* 1. Initialize twiddle factors */
    fft_twiddle_init_f32();
    fft_twiddle_init_q15();
    fft_twiddle_init_q31();
    
    /* 2. Generate synthetic signal and apply window */
    for (int i = 0; i < FFT_SIZE; i++) {
        float32_t t = (float32_t)i / SAMPLING_FREQ;
        float32_t val = 0.6f * sinf(2.0f * (float32_t)M_PI * SIGNAL_FREQ1 * t) + 
                        0.3f * sinf(2.0f * (float32_t)M_PI * SIGNAL_FREQ2 * t);
        
        /* Hann window: 0.5 * (1 - cos(2*pi*i/(N-1))) */
        float32_t w = 0.5f * (1.0f - cosf(2.0f * (float32_t)M_PI * (float32_t)i / (float32_t)(FFT_SIZE - 1)));
        float32_t win_val = val * w;

        data_f32[i].real = win_val;
        data_f32[i].imag = 0.0f;
        
        data_q15[i].real = (q15_t)(win_val * 32767.0f);
        data_q15[i].imag = 0;
        
        data_q31[i].real = (q31_t)(win_val * 2147483647.0f);
        data_q31[i].imag = 0;
    }
    
    /* 3. Perform FFTs */
    fft_f32_radix2(data_f32, FFT_SIZE, 0);
    fft_q15_radix2(data_q15, FFT_SIZE);
    fft_q31_radix2(data_q31, FFT_SIZE);
    
    /* 4. Compute magnitudes */
    fft_f32_magnitude(data_f32, mag_f32, FFT_SIZE);
    fft_q15_magnitude(data_q15, mag_q15, FFT_SIZE);
    fft_q31_magnitude(data_q31, mag_q31, FFT_SIZE);
    
    /* 5. Output comparison results to CSV */
    FILE *fp = fopen("fft_comparison.csv", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    fprintf(fp, "Frequency(Hz),Mag_F32,Mag_Q15_Scaled,Mag_Q31_Scaled\n");
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        float32_t freq = (float32_t)i * SAMPLING_FREQ / (float32_t)FFT_SIZE;
        
        /* Fixed point magnitudes are scaled by 1/N because of internal scaling at each stage */
        /* To compare with F32, we scale them back up */
        float32_t val_q15 = (float32_t)mag_q15[i] / 32767.0f * (float32_t)FFT_SIZE;
        float32_t val_q31 = (float32_t)mag_q31[i] / 2147483647.0f * (float32_t)FFT_SIZE;
        
        fprintf(fp, "%.2f,%.4f,%.4f,%.4f\n", freq, mag_f32[i], val_q15, val_q31);
    }
    
    fclose(fp);
    printf("Simulation completed (with Hann windowing). Results saved to fft_comparison.csv\n");
    
    /* Quick summary for peaks */
    printf("\nComparison at Peak Bins (Windowed):\n");
    printf("Bin  Freq      F32       Q15       Q31\n");
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        if (mag_f32[i] > 25.0f) { // Threshold lower due to windowing energy spread
            float32_t freq = (float32_t)i * SAMPLING_FREQ / (float32_t)FFT_SIZE;
            float32_t val_q15 = (float32_t)mag_q15[i] / 32767.0f * (float32_t)FFT_SIZE;
            float32_t val_q31 = (float32_t)mag_q31[i] / 2147483647.0f * (float32_t)FFT_SIZE;
            printf("%3d  %6.2f Hz  %8.2f  %8.2f  %8.2f\n", i, freq, mag_f32[i], val_q15, val_q31);
        }
    }

    return 0;
}
