#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cstdlib>

extern "C" {
    #include "fft_f32.h"
    #include "fft_q15.h"
    #include "fft_q31.h"
    #include "fft_twiddle.h"
    #include "fft_utils.h"
    #include "fft_window.h"
}

using namespace std;

const float SAMPLING_FREQ = 1000.0f;
const float PI = 3.14159265358979323846f;

void run_gnuplot(const string& script) {
    string command = "gnuplot -e \"" + script + "\"> /dev/null 2>&1";
    int ret = system(command.c_str());
    if (ret != 0) {
        cerr << "Warning: Gnuplot command failed or was not found." << endl;
    }
}

int main() {
    fft_twiddle_init_f32();
    fft_twiddle_init_q15();
    fft_twiddle_init_q31();
    
    system("mkdir -p results");

    /* --- CHAPTER 1: THE ZERO PADDING EFFECT --- */
    /* Signal: 50Hz + 52Hz (Very close together) */
    /* Observe: In 512 samples, they are merged. Does padding to 2048 separate them? */
    
    uint32_t num_samples = 512;
    uint32_t fft_size_normal = 512;
    uint32_t fft_size_padded = 2048;

    vector<float> signal_data(num_samples);
    for (uint32_t i = 0; i < num_samples; i++) {
        float t = (float)i / SAMPLING_FREQ;
        // Two very close frequencies
        signal_data[i] = 0.6f * sinf(2.0f * PI * 50.0f * t) + 
                         0.5f * sinf(2.0f * PI * 52.5f * t);
        // Apply Hann window
        signal_data[i] *= 0.5f * (1.0f - cosf(2.0f * PI * i / (float)(num_samples - 1)));
    }

    // 1. Normal FFT (N=512)
    vector<complex_f32_t> data_512(fft_size_normal);
    vector<float32_t> mag_512(fft_size_normal);
    for(uint32_t i=0; i<num_samples; i++) {
        data_512[i].real = signal_data[i];
        data_512[i].imag = 0.0f;
    }
    fft_f32_radix2(data_512.data(), fft_size_normal, 0);
    fft_f32_magnitude(data_512.data(), mag_512.data(), fft_size_normal);

    // 2. Zero-Padded FFT (N=2048)
    vector<complex_f32_t> data_2048(fft_size_padded, {0,0});
    vector<float32_t> mag_2048(fft_size_padded);
    for(uint32_t i=0; i<num_samples; i++) {
        data_2048[i].real = signal_data[i];
        data_2048[i].imag = 0.0f;
    }
    // Note: indexes num_samples to fft_size_padded are already 0.0
    fft_f32_radix2(data_2048.data(), fft_size_padded, 0);
    fft_f32_magnitude(data_2048.data(), mag_2048.data(), fft_size_padded);

    /* Save results for comparison */
    ofstream padData("results/padding_comparison.dat");
    // We only plot 40Hz to 65Hz to see the resolution detail
    for (float f = 40.0f; f <= 65.0f; f += 0.1f) {
        int bin_512 = (int)(f * fft_size_normal / SAMPLING_FREQ);
        int bin_2048 = (int)(f * fft_size_padded / SAMPLING_FREQ);
        padData << f << " " << mag_512[bin_512] << " " << mag_2048[bin_2048] << endl;
    }
    padData.close();

    cout << "Generating Zero-Padding Comparison Plot (results/padding_effect.png)..." << endl;
    run_gnuplot("set terminal pngcairo size 800,600; "
                "set output 'results/padding_effect.png'; "
                "set title 'The Zero-Padding Effect (Digital Zoom)'; "
                "set xlabel 'Frequency (Hz)'; set ylabel 'Magnitude'; "
                "set grid; "
                "plot 'results/padding_comparison.dat' using 1:2 with linespoints title 'Normal (N=512)' lc rgb 'red', "
                "     'results/padding_comparison.dat' using 1:3 with lines title 'Padded (N=2048)' lc rgb 'blue' lw 2");

    /* --- FINAL REPORT --- */
    ofstream reportFile("results/fft_report.txt");
    reportFile << "====================================================" << endl;
    reportFile << "        ZERO PADDING AND INTERPOLATION REPORT" << endl;
    reportFile << "====================================================" << endl << endl;
    reportFile << "Observation 1: The 'Digital Zoom' Effect" << endl;
    reportFile << "----------------------------------------" << endl;
    reportFile << "Signal: 50.0 Hz and 52.5 Hz (Difference = 2.5 Hz)" << endl;
    reportFile << "At Fs=1000Hz, N=512, Resolution = 1.95 Hz." << endl << endl;
    reportFile << "- Red Line (Normal N=512): The spectrum is 'jagged'. The two peaks are merged into one flat-top blob." << endl;
    reportFile << "- Blue Line (Padded N=2048): The spectrum is 'smooth'. It looks like it has higher resolution," << endl;
    reportFile << "  BUT notice that it still shows ONE merged peak. Zero-padding DID NOT separate the frequencies." << endl << endl;
    reportFile << "Conclusion for Hardware (STM32):" << endl;
    reportFile << "Zero padding is useful for making pretty graphs and finding the peak frequency more accurately," << endl;
    reportFile << "but it does NOT replace the need for a longer time buffer if you need to distinguish close tones." << endl;
    reportFile.close();

    cout << "Simulation complete. Check 'results/padding_effect.png' for the visualization." << endl;

    return 0;
}
