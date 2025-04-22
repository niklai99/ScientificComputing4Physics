#include "FFT.hpp"
#include <algorithm>
#include <cmath>

namespace FFT {

// ————————————————————————————————————————————————————————————————
// next_power_of_two
// ————————————————————————————————————————————————————————————————
std::size_t next_power_of_two(std::size_t n) {
    std::size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}

// ————————————————————————————————————————————————————————————————
// Primitive in‑place Cooley–Tukey FFT
// ————————————————————————————————————————————————————————————————
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

// ————————————————————————————————————————————————————————————————
// Internal 1D FFT on power‑of‑two length (by value)
// ————————————————————————————————————————————————————————————————
static std::vector<std::complex<double>>
fft1d(std::vector<std::complex<double>> a, bool invert) {
    fft(a, invert);
    return a;
}

// ————————————————————————————————————————————————————————————————
// 2D FFT (pads rows×cols to powers of two, then FFT rows and cols)
// ————————————————————————————————————————————————————————————————
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

// ————————————————————————————————————————————————————————————————
// Real→complex “trimmed” 2D FFT
// ————————————————————————————————————————————————————————————————
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

// ————————————————————————————————————————————————————————————————
// Inverse real→complex→real 2D FFT (Hermitian symmetry)
// ————————————————————————————————————————————————————————————————
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

} // namespace FFT
