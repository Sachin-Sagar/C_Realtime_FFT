# Hardware Limitation Sweep Plan

## Objective
Build a C++ test harness to evaluate the C FFT library under emulated hardware constraints.

## Architecture Updates
- Core C implementation remains unchanged.
- New C++ harness in the sim directory.
- Update Makefile to link C and C++ objects.

## Test Scenarios
1. Resolution vs FFT Size: Find the minimum N to separate close frequencies.
2. Dynamic Range: Compare F32 and Q15 with weak signals.
3. ADC Emulation: Test 10-bit and 12-bit bit-depths.

## Reporting
- Generate a summary report in the terminal.