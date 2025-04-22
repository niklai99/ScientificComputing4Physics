// test/test_fft2d_r2c.cpp

#include <cassert>
#include <iostream>
#include <vector>
#include "FFT.hpp"
#include "FFTTestHelpers.hpp"

int main() {
    using std::vector;
    using FFT::fft2d_r2c_trim;
    using FFT::ifft2d_c2r_trim;
    using FFT::next_power_of_two;

    // small real 2×3 matrix
    vector<vector<double>> A{
        {1,2,3},
        {4,5,6}
    };

    // forward real‑to‑complex: pad 3→4, keep half‑spectrum 4/2+1=3
    auto R = fft2d_r2c_trim(A);
    size_t rows    = A.size();                 // 2
    size_t cols    = A[0].size();              // 3
    size_t padded  = next_power_of_two(cols);  // 4
    size_t freqCols= padded/2 + 1;             // 3

    assert(R.size()    == rows);
    assert(R[0].size() == freqCols);
    std::cout << "✔ fft2d_r2c_trim: freq‑domain shape OK\n";

    // inverse back to real
    auto Arec = ifft2d_c2r_trim(R, cols);
    assert(Arec.size()    == rows);
    assert(Arec[0].size() == cols);
    assert(approx_eq(Arec, A));
    std::cout << "✔ fft2d_r2c_trim + ifft2d_c2r_trim round‑trip\n";

    return 0;
}
