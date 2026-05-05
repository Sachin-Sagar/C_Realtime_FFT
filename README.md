# Real-Time FFT in Pure C

This project provides a portable, pure C implementation of the Fast Fourier Transform (FFT) for 32-bit microcontrollers (e.g., STM32). It includes both floating-point and fixed-point (Q15, Q31) variants.

## Features
- **Algorithm:** Radix-2 Decimation-In-Time (DIT) Cooley-Tukey.
- **Arithmetic:** Floating-Point (Single Precision f32) and Fixed-Point (Q15, Q31).
- **Windowing:** Hann, Hamming, and Blackman windows.
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
This generates high-quality plots and reports in the `results/` directory:
- **`results/fft_report.txt`**: A comprehensive text report featuring a comparison of peak bins.
- **`results/time_domain.png`**: Plot of the input time-series signal.
- **`results/frequency_domain.png`**: Plot of the FFT magnitude spectrum.
- **`results/fft_comparison.csv`**: Raw high-resolution data for all three FFT variants (F32, Q15, Q31), suitable for external plotting. Note: `make clean` does not delete this file.

### 2. Hardware Limitation Sweep
To analyze the impact of microcontroller hardware constraints (resolution, bit-depth, and quantization noise):
```bash
make sweep
```
This runs an automated suite that sweeps through different FFT sizes and ADC bit-depths, helping you determine the minimum requirements for your specific target hardware.

## Usage on Microcontrollers
1. Copy `src/` and `include/` files to your project.
2. Initialize twiddle factors once at startup:
   ```c
   fft_twiddle_init_f32(); // or fft_twiddle_init_q15(), etc.
   ```
3. Prepare your complex data and call the desired FFT function:
   ```c
   fft_f32_radix2(data, 1024, 0); // Forward FFT
   ```

# C_Realtime_FFT
