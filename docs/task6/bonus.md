In the bonus section, we verify that when using R2C FFT we can use Hermitian symmetry to recover the full complex spectrum exactly, using a small $6 \times 6$ test matrix. The driver is `task06_bonus.cpp`.

## Test driver

The main function in `task06_bonus.cpp` orchestrates: generation of test data, full C2C FFT, half-spectrum R2C FFT and Hermitian reconstruction, and spectrum comparison after reconstruction with Hermitian symmetry.

```cpp linenums="10" title="task06_bonus.cpp"
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
```

## Hermitian reconstruction

The key function `r2c_reconstruct_full` in `FFT.cpp` reconstructs the full complex spectrum C from the half-spectrum R. It uses Hermitian symmetry to fill in the missing frequencies:

```cpp linenums="202" title="FFT.cpp"
std::vector<std::vector<std::complex<double>>>
r2c_reconstruct_full(const std::vector<std::vector<std::complex<double>>>& R)
{
    std::size_t M      = R.size();
    std::size_t N_half = M && !R.empty() ? R[0].size() : 0;
    std::size_t N      = 2*(N_half - 1);

    // rebuild full Hermitian spectrum
    std::vector<std::vector<std::complex<double>>> full(
        M, std::vector<std::complex<double>>(N));
    for (std::size_t i = 0; i < M; ++i) {
        // first half
        for (std::size_t j = 0; j < N_half; ++j)
            full[i][j] = R[i][j];
        // mirror
        for (std::size_t j = N_half; j < N; ++j) {
            std::size_t ii = (M - i) % M;
            std::size_t jj = (N - j) % N;
            full[i][j] = std::conj(R[ii][jj]);
        }
    }
    return full;
}
```


## Expected outcome 

Because Hermitian reconstruction implements exactly the inverse of the R2C trimming process, the reconstructed spectrum `C_from_R` should match the direct C2C FFT `C_full` within round-off error. On the 6×6 test, typical results are:

```plaintext
Max abs(C_full - C_from_R) = 7.10543e-15
```

This confirms that our R2C trimming and reconstruction are mathematically correct and numerically robust.