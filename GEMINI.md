# Real-Time FFT in Pure C - Project Instructions

## Project Overview
This project is a portable, pure C implementation of the Fast Fourier Transform (FFT) optimized for 32-bit microcontrollers (e.g., STM32). It avoids external dependencies beyond the C standard library and provides multiple arithmetic variants to suit different hardware capabilities.

### Key Technologies
- **Language:** C (C99/C11 compatible)
- **Algorithms:** Radix-2 Decimation-In-Time (DIT) Cooley-Tukey FFT.
- **Arithmetic Variants:** 
    - Floating-Point (Single Precision `f32`)
    - Fixed-Point (Q15 and Q31 formats)

### Architecture
- **`include/`**: Public API headers defining data structures and function prototypes.
- **`src/`**: Implementation files for the FFT core, twiddle factors, and utilities.
- **`sim/`**: A C++ simulation harness used for algorithm verification and benchmarking.
- **`plans/`**: Documentation of the implementation strategy.

## Building and Running

### PC Simulation
The project includes a Makefile for building the simulation environment on a PC.

- **Build and Run Simulation:**
  ```bash
  make run
  ```
  This compiles the source code, runs the simulation, and outputs a comprehensive visual report to `fft_report.txt` and raw comparison data to `fft_comparison.csv`.

- **Clean Build Artifacts:**
  ```bash
  make clean
  ```

### Microcontroller Integration
1.  Copy the contents of `include/` and `src/` to your MCU project.
2.  Include the relevant headers (e.g., `fft_f32.h`, `fft_q15.h`).
3.  **Mandatory Initialization:** Call the appropriate twiddle factor initialization function once at startup:
    - `fft_twiddle_init_f32()`
    - `fft_twiddle_init_q15()`
    - `fft_twiddle_init_q31()`

## Development Conventions

### Coding Style
- **Naming:** Follows a consistent prefixing convention: `fft_<variant>_<function>`.
- **Data Types:** Uses explicit width types from `<stdint.h>` (e.g., `uint32_t`, `int16_t`).
- **Memory Management:** Strictly avoids dynamic allocation (`malloc`). All operations are performed **in-place** on the provided data buffers to minimize memory footprint.

### Arithmetic Logic
- **Fixed-Point Scaling:** The Q15 and Q31 implementations include internal scaling (right-shifting) at each butterfly stage to prevent overflow during intermediate calculations.
- **Twiddle Factors:** Performance is optimized using pre-computed Look-Up Tables (LUTs) initialized at runtime via the `fft_twiddle_init_*` functions.

### Testing
- Verification is performed via the simulation harness in `sim/main_sim.c`, which compares the output of fixed-point variants against the floating-point reference.
- Any new features or optimizations should be validated using the simulation to ensure no regression in precision or correctness.
