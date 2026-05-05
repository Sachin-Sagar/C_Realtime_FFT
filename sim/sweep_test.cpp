#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

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

// Helper to find peaks in a magnitude array
struct Peak {
    int bin;
    float freq;
    float magnitude;
};

vector<Peak> find_peaks(const float* magnitude, int n, float threshold) {
    vector<Peak> peaks;
    for (int i = 1; i < (int)n / 2 - 1; i++) {
        if (magnitude[i] > threshold && magnitude[i] > magnitude[i-1] && magnitude[i] > magnitude[i+1]) {
            peaks.push_back({i, (float)i * SAMPLING_FREQ / n, magnitude[i]});
        }
    }
    return peaks;
}

void run_resolution_test(ostream& out) {
    out << "--- Test A: Resolution vs FFT Size (50Hz vs 55Hz) ---" << endl;
    out << setw(6) << "N" << setw(15) << "Resolution" << setw(15) << "Status" << endl;
    out << string(40, '-') << endl;

    vector<uint32_t> sizes = {64, 128, 256, 512, 1024};
    float f1 = 50.0f;
    float f2 = 55.0f;

    for (uint32_t n : sizes) {
        vector<complex_f32_t> data(n);
        vector<float32_t> magnitude(n);

        for (uint32_t i = 0; i < n; i++) {
            float t = (float)i / SAMPLING_FREQ;
            data[i].real = sinf(2.0f * PI * f1 * t) + sinf(2.0f * PI * f2 * t);
            data[i].imag = 0.0f;
        }

        fft_f32_radix2(data.data(), n, 0);
        fft_f32_magnitude(data.data(), magnitude.data(), n);

        auto peaks = find_peaks(magnitude.data(), n, 10.0f);
        
        bool resolved = false;
        if (peaks.size() >= 2) {
            bool found50 = false;
            bool found55 = false;
            for (const auto& p : peaks) {
                if (abs(p.freq - 50.0f) < 5.0f) found50 = true;
                if (abs(p.freq - 55.0f) < 5.0f) found55 = true;
            }
            if (found50 && found55) resolved = true;
        }

        out << setw(6) << n << setw(15) << fixed << setprecision(2) << SAMPLING_FREQ / n << " Hz" 
             << setw(15) << (resolved ? "RESOLVED" : "MERGED") << endl;
    }
    out << endl;
}

void run_dynamic_range_test(ostream& out) {
    out << "--- Test B: Dynamic Range (F32 vs Q15) ---" << endl;
    uint32_t n = 1024;
    float f_strong = 50.0f;
    float f_weak = 120.0f;
    float a_weak = 0.001f;

    out << "Strong Signal: 1.0 @ 50Hz, Weak Signal: " << a_weak << " @ 120Hz" << endl;
    out << setw(10) << "Format" << setw(15) << "Weak Peak Found" << setw(15) << "Weak Mag" << endl;
    out << string(45, '-') << endl;

    // F32
    {
        vector<complex_f32_t> data(n);
        vector<float32_t> magnitude(n);
        for (uint32_t i = 0; i < n; i++) {
            float t = (float)i / SAMPLING_FREQ;
            data[i].real = sinf(2.0f * PI * f_strong * t) + a_weak * sinf(2.0f * PI * f_weak * t);
            data[i].imag = 0.0f;
        }
        fft_f32_radix2(data.data(), n, 0);
        fft_f32_magnitude(data.data(), magnitude.data(), n);
        auto peaks = find_peaks(magnitude.data(), n, 0.1f);
        bool found = any_of(peaks.begin(), peaks.end(), [&](const Peak& p){ return abs(p.freq - 120.0f) < 2.0f; });
        float mag = 0;
        for(auto& p : peaks) if(abs(p.freq - 120.0f) < 2.0f) mag = p.magnitude;
        out << setw(10) << "F32" << setw(15) << (found ? "YES" : "NO") << setw(15) << fixed << setprecision(4) << mag << endl;
    }

    // Q15
    {
        vector<complex_q15_t> data(n);
        vector<q15_t> magnitude(n);
        for (uint32_t i = 0; i < n; i++) {
            float t = (float)i / SAMPLING_FREQ;
            float val = sinf(2.0f * PI * f_strong * t) + a_weak * sinf(2.0f * PI * f_weak * t);
            data[i].real = (q15_t)(val * 32767.0f);
            data[i].imag = 0;
        }
        fft_q15_radix2(data.data(), n);
        fft_q15_magnitude(data.data(), magnitude.data(), n);
        vector<float> f_mag(n);
        for(uint32_t i=0; i<n; i++) f_mag[i] = (float)magnitude[i] / 32767.0f * n;
        auto peaks = find_peaks(f_mag.data(), n, 0.1f);
        bool found = any_of(peaks.begin(), peaks.end(), [&](const Peak& p){ return abs(p.freq - 120.0f) < 2.0f; });
        float mag = 0;
        for(auto& p : peaks) if(abs(p.freq - 120.0f) < 2.0f) mag = p.magnitude;
        out << setw(10) << "Q15" << setw(15) << (found ? "YES" : "NO") << setw(15) << fixed << setprecision(4) << mag << endl;
    }
    out << "Note: Q15 internal scaling reduces effective bit depth for weak signals." << endl << endl;
}

void run_adc_depth_test(ostream& out) {
    out << "--- Test C: ADC Bit-Depth Emulation (1024 pt, Hann) ---" << endl;
    uint32_t n = 1024;
    float f = 50.0f;

    out << setw(10) << "Bits" << setw(15) << "Noise Floor" << endl;
    out << string(30, '-') << endl;

    vector<int> depths = {8, 10, 12, 16, 24};

    for (int bits : depths) {
        vector<complex_f32_t> data(n);
        vector<float32_t> magnitude(n);
        float levels = pow(2.0f, bits - 1);

        for (uint32_t i = 0; i < n; i++) {
            float t = (float)i / SAMPLING_FREQ;
            float val = 0.8f * sinf(2.0f * PI * f * t);
            val = floorf(val * levels + 0.5f) / levels;
            float w = 0.5f * (1.0f - cosf(2.0f * PI * i / (n - 1)));
            data[i].real = val * w;
            data[i].imag = 0.0f;
        }

        fft_f32_radix2(data.data(), n, 0);
        fft_f32_magnitude(data.data(), magnitude.data(), n);

        float noise_sum = 0;
        int count = 0;
        for (uint32_t i = 0; i < n / 2; i++) {
            float freq = (float)i * SAMPLING_FREQ / n;
            if (abs(freq - f) > 10.0f) {
                noise_sum += magnitude[i];
                count++;
            }
        }
        out << setw(10) << bits << setw(15) << fixed << setprecision(6) << noise_sum / count << endl;
    }
}

int main() {
    fft_twiddle_init_f32();
    fft_twiddle_init_q15();
    fft_twiddle_init_q31();

    system("mkdir -p results");
    ofstream reportFile("results/sweep_report.txt");

    cout << "Running Hardware Limitation Sweep..." << endl;

    reportFile << "====================================================" << endl;
    reportFile << "   REAL-TIME FFT HARDWARE LIMITATION SWEEP" << endl;
    reportFile << "====================================================" << endl << endl;

    run_resolution_test(reportFile);
    run_dynamic_range_test(reportFile);
    run_adc_depth_test(reportFile);

    reportFile << endl << "====================================================" << endl;
    reportFile.close();

    cout << "Sweep complete. Results saved to 'results/sweep_report.txt'." << endl;
    return 0;
}
