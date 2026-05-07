# Real-Time FFT in Pure C

This project provides a portable, pure C implementation of the Fast Fourier Transform (FFT) for 32-bit microcontrollers (e.g., STM32). It includes both floating-point and fixed-point (Q15, Q31) variants.

## Features
- **Algorithm:** Radix-2 Decimation-In-Time (DIT) Cooley-Tukey.
- **Arithmetic:** Floating-Point (Single Precision f32) and Fixed-Point (Q15, Q31).
- **Windowing:** Hann, Hamming, and Blackman windows.
- **DMA Pipeline:** High-performance "out-of-place" processing specifically for STM32 DMA buffers, combining conversion, centering, reordering, and FFT in one pass.
- **Optimization:** Pre-computed twiddle factor LUTs initialized at runtime.
- **Portability:** Dependency-free (standard C library only).
- **Efficiency:** In-place operations to minimize memory footprint.

## Project Structure
- `include/`: API headers and internal configuration.
- `src/`: Core FFT algorithms, twiddle factors, and utilities (C).
- `sim/`: C++ simulation harness and hardware emulation tools.
- `Makefile`: Build system for simulation and analysis.

## Simulation and Analysis

### 1. Basic Simulation
To build and run the main simulation harness:
```bash
make run
```
This generates a comprehensive suite of visualizations and reports in the `results/` directory:
- **`results/fft_report.txt`**: Analysis of zero-padding vs. physical resolution.
- **`results/time_domain.png`**: Visualization of the standard test signal (60Hz + 120Hz + Noise).
- **`results/frequency_domain.png`**: Magnitude spectrum showing clearly resolved peaks.
- **`results/padding_effect.png`**: A side-by-side comparison of normal vs. zero-padded FFTs, demonstrating the "Digital Zoom" effect.
- **`results/*.dat`**: Raw data files (time, frequency, padding) for custom analysis or plotting.

### 2. Hardware Limitation Sweep
To analyze the impact of microcontroller hardware constraints (resolution, bit-depth, and quantization noise):
```bash
make sweep
```
This runs an automated suite that sweeps through different FFT sizes and ADC bit-depths, helping you determine the minimum requirements for your specific target hardware. Results are saved to **`results/sweep_report.txt`**.

## Usage on Microcontrollers
1. Copy `src/` and `include/` files to your project.
2. Initialize twiddle factors once at startup:
   ```c
   fft_twiddle_init_f32(); // or fft_twiddle_init_q15(), etc.
   ```
3. **Standard Usage:** Prepare your complex data and call the desired FFT function:
   ```c
   fft_f32_radix2(data, 1024, 0); // Forward FFT
   ```
4. **DMA Pipeline (Recommended for STM32):** Process raw ADC data directly into a complex FFT array:
   ```c
   #include "fft_pipeline.h"
   // Convert, Center, Shuffle, and Compute in one call
   fft_pipeline_process_f32(adc_dma_buffer, fft_complex_buffer, 1024, 2048.0f);
   ```

# C_Realtime_FFT
