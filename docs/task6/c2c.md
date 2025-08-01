# Complex-to-complex FFT implementation

This page examines in detail how the code performs the 2D complex-to-complex FFT, including padding, the 1D FFT core, and the inverse with trimming.

## Padding to power-of-two dimensions

Before applying the FFT, the input matrix is padded so that both dimensions are powers of two. In `FFT.cpp`, the helper `next_power_of_two` and padding loop are used:

```cpp linenums="10" title="FFT.cpp"
std::size_t next_power_of_two(std::size_t n) {
    std::size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}
```

```cpp linenums="68" title="inside FFT::fft2d(...)"
std::size_t R = input.size(); // number of rows
std::size_t C = R ? input[0].size() : 0; // number of columns
std::size_t M = next_power_of_two(R);
std::size_t N = next_power_of_two(C);

// pad to M×N
std::vector<std::vector<std::complex<double>>> a(
    M, std::vector<std::complex<double>>(N, {0,0}));
for (std::size_t i = 0; i < R; ++i)
    for (std::size_t j = 0; j < C; ++j)
        a[i][j] = input[i][j];
```

This ensures optimal FFT speed and simplifies the Cooley–Tukey recursion.

## 1D FFT core

The 1D FFT uses an in-place Cooley–Tukey algorithm with bit reversal and iterative butterflies:

```cpp linenums="19" title="FFT.cpp"
void fft(std::vector<std::complex<double>>& a, bool invert) {
    const std::size_t n = a.size();
    if (n < 2) return;

    // bit‑reversal permute
    for (std::size_t i = 1, j = 0; i < n; ++i) {
        std::size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j |= bit;
        if (i < j) std::swap(a[i], a[j]);
    }

    // Cooley–Tukey butterflies
    for (std::size_t len = 2; len <= n; len <<= 1) {
        double ang = 2 * M_PI / double(len) * (invert ? 1 : -1);
        std::complex<double> wlen(std::cos(ang), std::sin(ang));
        for (std::size_t i = 0; i < n; i += len) {
            std::complex<double> w{1,0};
            for (std::size_t k = 0; k < len/2; ++k) {
                auto u = a[i + k];
                auto v = a[i + k + len/2] * w;
                a[i + k]         = u + v;
                a[i + k + len/2] = u - v;
                w *= wlen;
            }
        }
    }

    // scale for inverse
    if (invert) {
        for (auto& x : a) x /= double(n);
    }
}
```

This routine underlies both forward and inverse transforms.


## 2D FFT implementation

The 2D FFT applies fft row-wise, transposes, and applies fft again row-wise (on columns):

```cpp linenums="65" title="FFT.cpp"
std::vector<std::vector<std::complex<double>>>
fft2d(const std::vector<std::vector<std::complex<double>>>& input,
      bool invert)
{
    std::size_t R = input.size();
    std::size_t C = R ? input[0].size() : 0;
    std::size_t M = next_power_of_two(R);
    std::size_t N = next_power_of_two(C);

    // pad to M×N
    std::vector<std::vector<std::complex<double>>> a(
        M, std::vector<std::complex<double>>(N, {0,0}));
    for (std::size_t i = 0; i < R; ++i)
        for (std::size_t j = 0; j < C; ++j)
            a[i][j] = input[i][j];

    // FFT rows
    for (std::size_t i = 0; i < M; ++i)
        a[i] = fft1d(a[i], invert);

    // FFT cols
    std::vector<std::complex<double>> tmp(M);
    for (std::size_t j = 0; j < N; ++j) {
        for (std::size_t i = 0; i < M; ++i) tmp[i] = a[i][j];
        tmp = fft1d(tmp, invert);
        for (std::size_t i = 0; i < M; ++i) a[i][j] = tmp[i];
    }

    return a;
}
```

This decomposes the 2D problem into  calls to the 1D FFT.


## Complex-to-complex FFT 

Rather than returning a bare matrix, fft2d_c2c_trim returns a struct that encapsulates both the frequency-domain data and the original dimensions. In `FFT.hpp`:

```cpp linenums="56" title="FFT.hpp"
struct FFT2dC2CTrimmed {
    std::vector<std::vector<std::complex<double>>> freq;
    std::size_t orig_rows, orig_cols;
    std::size_t pad_rows, pad_cols;
};

FFT2dC2CTrimmed
fft2d_c2c_trim(const std::vector<std::vector<std::complex<double>>>& input);
```

In `FFT.cpp`, the implementation captures the full padded transform and records the original size:

```cpp linenums="166" title="FFT.cpp"
FFT2dC2CTrimmed
fft2d_c2c_trim(const std::vector<std::vector<std::complex<double>>>& input)
{
    const std::size_t R = input.size();
    const std::size_t C = R ? input[0].size() : 0;
    const std::size_t M = next_power_of_two(R);
    const std::size_t N = next_power_of_two(C);

    // 1) compute the full M×N forward transform
    auto full = fft2d(input, /*invert=*/false);

    // 2) package it all up
    return FFT2dC2CTrimmed{ std::move(full), R, C, M, N };
}
```

The padded transform `full` has dimensions that are powers of two, which may exceed the original. By retaining `orig_rows` and `orig_cols`, the inverse transform can know exactly how to crop the result back to the user’s intended size.


## Inverse complex-to-complex FFT

To return to original dimensions, `ifft2d_c2c_trim` accepts the `FFT2dC2CTrimmed` struct and uses its metadata:

```cpp linenums="181" title="FFT.cpp"
std::vector<std::vector<std::complex<double>>>
ifft2d_c2c_trim(const FFT2dC2CTrimmed& t)
{
    // 1) full padded inverse
    auto fullRec = fft2d(t.freq, /*invert=*/true);

    // 2) crop back to original R×C
    std::vector<std::vector<std::complex<double>>> out(
        t.orig_rows,
        std::vector<std::complex<double>>(t.orig_cols)
    );
    for (std::size_t i = 0; i < t.orig_rows; ++i)
        for (std::size_t j = 0; j < t.orig_cols; ++j)
            out[i][j] = fullRec[i][j];

    return out;
}
```

By combining the padded spectrum and the stored original sizes, this function ensures the round-trip FFT returns exactly to the user’s matrix dimensions.