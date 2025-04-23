#ifndef FFT_HPP
#define FFT_HPP

#include <vector>
#include <complex>
#include <cstddef>

namespace FFT {

/**
 * @brief Returns the smallest power of two ≥ n.
 */
std::size_t next_power_of_two(std::size_t n);

/**
 * @brief In‑place Cooley–Tukey FFT:
 *        - forward if invert==false
 *        - inverse (and divides by n) if invert==true
 */
void fft(std::vector<std::complex<double>>& a, bool invert);

/**
 * @brief 2D FFT that pads both dims to powers of two,
 *        applies FFT on rows then cols, and returns the full M×N.
 */
std::vector<std::vector<std::complex<double>>>
fft2d(const std::vector<std::vector<std::complex<double>>>& input,
      bool invert);

/**
 * @brief Real‑to‑complex 2D FFT “trimmed” wrapper:
 *   1) promote real → complex,
 *   2) run full padded forward FFT,
 *   3) crop to M'×(N'/2+1).
 */
std::vector<std::vector<std::complex<double>>>
fft2d_r2c_trim(const std::vector<std::vector<double>>& input);

/**
 * @brief Inverse of the above real→complex transform:
 *   1) reconstruct full Hermitian M'×N' spectrum,
 *   2) run full padded inverse FFT,
 *   3) crop to real M×orig_cols.
 */
std::vector<std::vector<double>>
ifft2d_c2r_trim(const std::vector<std::vector<std::complex<double>>>& R,
                std::size_t orig_cols);


/**
 * @brief “Trimmed” complex‑to‑complex 2D FFT:
 *   1) run full padded forward FFT,
 *   2) crop top‑left orig_rows×orig_cols block,
 *   3) return plus metadata for inversion.
 */
struct FFT2dC2CTrimmed {
    std::vector<std::vector<std::complex<double>>> freq;
    std::size_t orig_rows, orig_cols;
    std::size_t pad_rows, pad_cols;
};

FFT2dC2CTrimmed
fft2d_c2c_trim(const std::vector<std::vector<std::complex<double>>>& input);

/**
 * @brief Inverse of fft2d_c2c_trim:
 *   1) re‑pad the cropped spectrum to pad_rows×pad_cols,
 *   2) run full padded inverse FFT,
 *   3) crop to orig_rows×orig_cols.
 */
std::vector<std::vector<std::complex<double>>>
ifft2d_c2c_trim(const FFT2dC2CTrimmed& t);


/**
 * @brief Reconstruct the full complex‐to‐complex spectrum from a
 *        real→complex “trimmed” half‐spectrum.
 *
 * Given R of shape M×(N/2+1), returns the full M×N Hermitian spectrum.
 */
std::vector<std::vector<std::complex<double>>>
r2c_reconstruct_full(const std::vector<std::vector<std::complex<double>>>& R);

} // namespace FFT

#endif // FFT_HPP
