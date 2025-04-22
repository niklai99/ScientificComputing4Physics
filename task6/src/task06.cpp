// src/task06.cpp

#include <iostream>
#include "Task06Helpers.hpp"
#include "FFT.hpp"

int main() {
    constexpr size_t ROWS = 1000, COLS = 1000;

    // 1) Generate A ~ N(1,1)
    auto A = generate_gaussian_matrix(ROWS, COLS, /*mean=*/1.0, /*stddev=*/1.0);

    // 2) Promote to complex
    auto Ac = promote_to_complex(A);

    // 3) c2c FFT + inverse using the trimmed interface
    auto trimmed = FFT::fft2d_c2c_trim(Ac);
    const auto& C_trim = trimmed.freq;              // M×N trimmed spectrum
    auto Arec_c2c     = FFT::ifft2d_c2c_trim(trimmed);

    // 4) Compute & print error statistics
    auto stats_c2c = evaluate_c2c_roundtrip(A, Arec_c2c);
    print_error_stats("c2c_trim round‑trip errors", stats_c2c);

    //    DC term lives in the (0,0) bin of the trimmed spectrum
    std::cout << "C[0][0] = " << C_trim[0][0]
              << "  (≈ sum of A)\n\n";

    // ---- now the r2c pathway ----

    // 5) r2c FFT + inverse (unchanged)
    auto R_half   = FFT::fft2d_r2c_trim(A);
    auto Arec_r2c = FFT::ifft2d_c2r_trim(R_half, COLS);

    // 6) Compute & print error statistics
    auto stats_r2c = evaluate_r2c_roundtrip(A, Arec_r2c);
    print_error_stats("r2c round‑trip errors", stats_r2c);

    std::cout << "R[0][0] = " << R_half[0][0]
              << "  (DC term again)\n\n";

    return 0;
}
