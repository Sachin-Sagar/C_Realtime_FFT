# Real-Time FFT in Pure C

This project provides a portable, pure C implementation of the Fast Fourier Transform (FFT) for 32-bit microcontrollers (e.g., STM32). It includes both floating-point and fixed-point (Q15, Q31) variants.

## Features
- Radix-2 Decimation-In-Time (DIT) Cooley-Tukey algorithm.
- Supports Floating-Point (Single Precision f32).
- Supports Fixed-Point (Q15 and Q31).
- Includes windowing functions (Hann, Hamming, Blackman).
- Optimized with pre-computed twiddle factor LUTs.
- Dependency-free (standard C library only).
- PC simulation harness for verification.

## Project Structure
- `include/`: API headers.
- `src/`: Implementation of FFT algorithms and utilities.
- `sim/`: PC simulation harness to verify correctness on synthetic signals.
- `Makefile`: Build system for simulation and library.

## Getting Started (PC Simulation)
To build and run the simulation:
```bash
make run
```
This will generate `fft_comparison.csv` and print a summary of peaks found using F32, Q15, and Q31 implementations.

## Usage on Microcontrollers
1. Add `src/` and `include/` files to your project.
2. Initialize twiddle factors once at startup:
   ```c
   fft_twiddle_init_f32(); // or fft_twiddle_init_q15(), etc.
   ```
3. Prepare your complex data and call the FFT function:
   ```c
   fft_f32_radix2(data, 1024, 0);
   ```
4. All functions are in-place for memory efficiency.
# C_Realtime_FFT
