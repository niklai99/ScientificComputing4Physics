// src/task06bonus.cpp

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include "Task06Helpers.hpp"
#include "FFT.hpp"

int main() {
    constexpr size_t ROWS = 6, COLS = 6;
    const double tol = 1e-12;

    // 1) generate A ~ N(1,1)
    auto A = generate_gaussian_matrix(ROWS, COLS, /*mean=*/1.0, /*stddev=*/1.0);

    // 2) promote to complex
    auto Ac = promote_to_complex(A);

    // 3) full padded c2c FFT
    auto C_full = FFT::fft2d(Ac, /*invert=*/false);

    // 4) trimmed real→complex
    auto R_half = FFT::fft2d_r2c_trim(A);

    // 5) reconstruct the full spectrum from R_half
    auto C_from_R = FFT::r2c_reconstruct_full(R_half);

    // 6) compare element‐wise
    size_t M = C_full.size(), N = C_full[0].size();
    double max_diff = 0.0;
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            max_diff = std::max(max_diff,
                                std::abs(C_full[i][j] - C_from_R[i][j]));

    std::cout << "Max abs(C_full - C_from_R) = " << max_diff << "\n";
    if (max_diff < tol) {
        std::cout << "✔ Bonus: reconstructed spectrum matches within tol = "
                  << tol << "\n";
        return 0;
    } else {
        std::cerr << "✘ Bonus: reconstruction error too large!\n";
        return 1;
    }
}
