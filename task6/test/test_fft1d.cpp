// test/test_fft1d.cpp

#include <cassert>
#include <iostream>
#include <vector>
#include <complex>
#include "FFT.hpp"
#include "FFTTestHelpers.hpp"

int main() {
    using std::vector;
    using std::complex;
    using FFT::fft;
    using FFT::next_power_of_two;

    // 1) delta‐function → all ones
    {
        vector<complex<double>> v{1,0,0,0};
        size_t m = next_power_of_two(v.size());  // should be 4
        v.resize(m, {0,0});
        fft(v, /*invert=*/false);
        for (auto& c : v)
            assert(approx_eq(c, complex<double>(1,0)));
        std::cout << "✔ fft 1D delta = all 1s\n";
    }

    // 2) known DFT of [1,2,3,4]
    {
        vector<complex<double>> v{1,2,3,4};
        size_t m = next_power_of_two(v.size());  // 4
        v.resize(m, {0,0});
        fft(v, /*invert=*/false);
        vector<complex<double>> exp{
            {10,0}, {-2,2}, {-2,0}, {-2,-2}
        };
        assert(approx_eq(v, exp));
        std::cout << "✔ fft 1D([1,2,3,4]) matches analytic DFT\n";
    }

    // 3) round‑trip forward→inverse with padding
    {
        vector<complex<double>> orig{5, -1, 0.5, 2.5, 7};
        size_t m = next_power_of_two(orig.size());  // 8
        vector<complex<double>> buf = orig;
        buf.resize(m, {0,0});
        fft(buf, /*invert=*/false);
        fft(buf, /*invert=*/true);

        // First orig.size() entries must match
        for (size_t i = 0; i < orig.size(); ++i)
            assert(approx_eq(buf[i], orig[i]));
        // The padded tail should be (near) zero
        for (size_t i = orig.size(); i < m; ++i)
            assert(approx_eq(buf[i], complex<double>(0,0)));

        std::cout << "✔ fft 1D forward+inverse round‑trip\n";
    }

    return 0;
}
