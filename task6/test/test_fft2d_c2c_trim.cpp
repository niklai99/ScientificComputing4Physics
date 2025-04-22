// test/test_fft2d_c2c_trim.cpp

#include <cassert>
#include <iostream>
#include <vector>
#include <complex>
#include "FFT.hpp"
#include "FFTTestHelpers.hpp"

int main() {
    using FFT::fft2d_c2c_trim;
    using FFT::ifft2d_c2c_trim;
    using FFT::next_power_of_two;

    // a tiny non‑power‑of‑two 2×3 matrix
    std::vector<std::vector<std::complex<double>>> A{
        {{1,0},{2,0},{3,0}},
        {{4,0},{5,0},{6,0}}
    };

    // run trimmed forward
    auto T = fft2d_c2c_trim(A);

    // metadata must match
    assert(T.orig_rows == A.size());
    assert(T.orig_cols == A[0].size());
    assert(T.pad_rows  == next_power_of_two(A.size()));
    assert(T.pad_cols  == next_power_of_two(A[0].size()));

    // freq block must be the full padded shape
    assert(T.freq.size()    == T.pad_rows);
    assert(T.freq[0].size() == T.pad_cols);

    std::cout << "✔ fft2d_c2c_trim: freq shape and metadata OK\n";

    // inverse back to original
    auto Arec = ifft2d_c2c_trim(T);
    assert(Arec.size()    == A.size());
    assert(Arec[0].size() == A[0].size());
    for (size_t i = 0; i < A.size(); ++i)
        for (size_t j = 0; j < A[0].size(); ++j)
            assert(approx_eq(Arec[i][j], A[i][j]));

    std::cout << "✔ fft2d_c2c_trim round‑trip\n";
    return 0;
}
