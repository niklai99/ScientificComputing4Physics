// test/test_fft2d_c2c.cpp

#include <cassert>
#include <iostream>
#include <vector>
#include <complex>
#include "FFT.hpp"
#include "FFTTestHelpers.hpp"

int main() {
    using std::vector;
    using std::complex;
    using FFT::fft2d;
    using FFT::next_power_of_two;

    // small non‑power‑of‑two 2×3 matrix
    vector<vector<complex<double>>> A{
        {{1,0},{2,0},{3,0}},
        {{4,0},{5,0},{6,0}}
    };

    // forward: pad to M×N = 2→2, 3→4
    auto C = fft2d(A, /*invert=*/false);
    size_t M = next_power_of_two(A.size());    // 2
    size_t N = next_power_of_two(A[0].size()); // 4
    assert(C.size()    == M);
    assert(C[0].size() == N);
    std::cout << "✔ fft2d: padded output shape OK ("<< M <<"×"<< N <<")\n";

    // inverse round‑trip: still M×N, but top-left block must match A
    auto R = fft2d(C, /*invert=*/true);
    assert(R.size()    == M);
    assert(R[0].size() == N);
    for (size_t i = 0; i < A.size(); ++i)
      for (size_t j = 0; j < A[0].size(); ++j)
        assert(approx_eq(R[i][j], A[i][j]));
    std::cout << "✔ fft2d padded round‑trip (top‑left block matches)\n";

    return 0;
}
