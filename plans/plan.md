# Implementation Plan: Real-Time FFT in Pure C

## 1. Background & Motivation
The goal is to develop a real-time Fast Fourier Transform (FFT) library in pure C for 32-bit microcontrollers (e.g., STM32). The library will exclusively use the C standard library to ensure portability across different MCU families and toolchains. To accommodate both older/cheaper MCUs without floating-point units (FPUs) and modern MCUs with FPUs, the library will provide both Fixed-Point (Q15 and Q31) and Floating-Point (Single Precision f32) variants. A PC-based simulation environment will be created to test and validate the algorithms before deployment to hardware.

## 2. Scope & Impact
- Implement Radix-2 Cooley-Tukey FFT algorithm.
- Provide both Floating-Point (f32) and Fixed-Point (q15, q31) implementations.
- Provide windowing functions (e.g., Hanning, Hamming) to reduce spectral leakage.
- Provide magnitude calculation from complex FFT output.
- Create a modular, dependency-free library structure.
- Develop a PC simulation harness to generate synthetic signals, process them, and output results.

## 3. Architecture & Directory Structure
The project will be organized as follows:

- include/: Public header files (config, f32, q15, q31, window, utils)
- src/: Source code implementation (logic for all variants, twiddle factors)
- sim/: PC Simulation & Testing (main harness, signal generation)
- Makefile: Build system for simulation & static lib
- README.md: Project documentation

## 4. Proposed Solution Details

### 4.1 Algorithms
- FFT Core: In-place Radix-2 Decimation-In-Time (DIT) Cooley-Tukey algorithm.
- Bit Reversal: Optimized bit-reversal sorting function.
- Twiddle Factors: Pre-computed look-up tables (LUTs) for twiddle factors to ensure real-time performance.
- Fixed-Point Arithmetic: q15_t (16-bit) and q31_t (32-bit) with careful shifting to prevent overflow.

### 4.2 Data Types
We will define standard types aligned with stdint.h. Complex numbers will be represented as structures with real and imaginary components.

### 4.3 PC Simulation (sim/)
- A main harness will generate synthetic signals (mix of frequencies and noise).
- It will apply window functions.
- It will run both f32 and q15/q31 FFT implementations.
- It will compute magnitudes and output results to a CSV file for verification.

## 5. Phased Implementation Plan

- Phase 1: Foundation & F32 Implementation (types, LUTs, Radix-2 f32 logic).
- Phase 2: Simulation Harness (signal gen, CSV output, verification).
- Phase 3: Fixed-Point Implementations (Q-math macros, q15/q31 logic).
- Phase 4: Optimization & Windowing (Hann/Hamming, profiling).

## 6. Alternatives Considered
- Radix-4 / Split-Radix: Faster but significantly more complex to implement and debug.
- Dynamic memory allocation: Explicitly avoided to ensure deterministic behavior on microcontrollers.

## 7. Migration & Rollback
This is a new application, so no migration is required. Accuracy issues in fixed-point will be addressed by adjusting scaling or recommending the floating-point version for FPU-equipped MCUs.