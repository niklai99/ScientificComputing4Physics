// test/test_fft2d_r2c_reconstruct_full.cpp

#include <cassert>
#include <iostream>
#include <vector>
#include <complex>
#include "FFT.hpp"
#include "FFTTestHelpers.hpp"

int main() {
    using FFT::fft2d;
    using FFT::fft2d_r2c_trim;
    using FFT::r2c_reconstruct_full;
    using FFT::next_power_of_two;

    // small real 2×3 matrix
    std::vector<std::vector<double>> A{
        {1,2,3},
        {4,5,6}
    };

    // manually promote to complex
    std::vector<std::vector<std::complex<double>>> Ac(2, std::vector<std::complex<double>>(3));
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 3; ++j)
            Ac[i][j] = { A[i][j], 0.0 };

    // compute full padded C
    auto C_full = fft2d(Ac, /*invert=*/false);

    // compute trimmed real→complex half‐spectrum
    auto R = fft2d_r2c_trim(A);

    // reconstruct full C from R
    auto C_rec = r2c_reconstruct_full(R);

    // expected padded dims
    size_t M = next_power_of_two(2);
    size_t N = next_power_of_two(3);
    assert(C_rec.size() == M);
    assert(C_rec[0].size() == N);

    // compare
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            assert(approx_eq(C_rec[i][j], C_full[i][j]));

    std::cout << "✔ fft2d_r2c_reconstruct_full: spectrum correctly reconstructed\n";
    return 0;
}
