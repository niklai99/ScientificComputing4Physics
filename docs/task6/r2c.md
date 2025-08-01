# Real-to-complex FFT implementation

This page details how the code performs the two-dimensional real-to-complex FFT (R2C), highlighting the differences and optimizations relative to the C2C pathway.

## Forward R2C FFT

Unlike the C2C transform, which operates on complex data, the R2C variant accepts a real-valued matrix and produces only the non-redundant half of the complex spectrum. The function signature in `FFT.hpp` is:

```cpp linenums="56" title="FFT.hpp"
std::vector<std::vector<std::complex<double>>>
fft2d_r2c_trim(const std::vector<std::vector<double>>& input);
```

In `FFT.cpp`, `fft2d_r2c_trim` is implemented by:

```cpp linenums="99" title="FFT.cpp"
std::vector<std::vector<std::complex<double>>>
fft2d_r2c_trim(const std::vector<std::vector<double>>& input)
{
    std::size_t R = input.size();
    std::size_t C = R ? input[0].size() : 0;

    // 1) promote to complex
    std::vector<std::vector<std::complex<double>>> ac(
        R, std::vector<std::complex<double>>(C));
    for (std::size_t i = 0; i < R; ++i)
        for (std::size_t j = 0; j < C; ++j)
            ac[i][j] = { input[i][j], 0.0 };

    // 2) full padded forward FFT
    auto full = fft2d(ac, /*invert=*/false);
    std::size_t M = full.size();
    std::size_t N = M ? full[0].size() : 0;
    std::size_t N_half = N/2 + 1;

    // 3) crop to M×(N/2+1)
    std::vector<std::vector<std::complex<double>>> out(
        M, std::vector<std::complex<double>>(N_half));
    for (std::size_t i = 0; i < M; ++i)
        for (std::size_t j = 0; j < N_half; ++j)
            out[i][j] = full[i][j];

    return out;
}
```

Extracting only the first `N/2 + 1` columns (columns 0 through `N/2`) we reduce memory and computation for real-valued data by nearly half.


## Hermitian spectrum reconstruction

To invert the R2C transform, we must first recreate the full complex spectrum from the trimmed output. The function signature in `FFT.hpp` is:

```cpp linenums="45" title="FFT.hpp"
std::vector<std::vector<double>>
ifft2d_c2r_trim(const std::vector<std::vector<std::complex<double>>>& R,
                std::size_t orig_cols);
```

Its implementation in `FFT.cpp` mirrors `fft2d_r2c_trim`:

```cpp linenums="131" title="FFT.cpp"
std::vector<std::vector<double>>
ifft2d_c2r_trim(const std::vector<std::vector<std::complex<double>>>& R,
                std::size_t orig_cols)
{
    std::size_t M      = R.size();
    std::size_t N_half = M && !R.empty() ? R[0].size() : 0;
    std::size_t N      = 2*(N_half - 1);

    // 1) reconstruct full Hermitian M×N
    std::vector<std::vector<std::complex<double>>> full(
        M, std::vector<std::complex<double>>(N));
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < N_half; ++j)
            full[i][j] = R[i][j];
        for (std::size_t j = N_half; j < N; ++j) {
            std::size_t ii = (M - i) % M;
            std::size_t jj = (N - j) % N;
            full[i][j] = std::conj(R[ii][jj]);
        }
    }

    // 2) full padded inverse FFT
    auto comp = fft2d(full, /*invert=*/true);

    // 3) crop to real M×orig_cols
    std::vector<std::vector<double>> out(
        M, std::vector<double>(orig_cols));
    for (std::size_t i = 0; i < M; ++i)
        for (std::size_t j = 0; j < orig_cols; ++j)
            out[i][j] = comp[i][j].real();

    return out;
}
```

The first step reconstructs the full complex matrix by applying Hermitian symmetry, where the second half of the spectrum is filled in by conjugating and reflecting the first half. The inverse FFT is then computed on this full matrix, and finally, we crop back to the original dimensions specified by `orig_cols`. 