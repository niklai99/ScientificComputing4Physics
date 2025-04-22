// File: test/include/FFTTestHelpers.hpp
#ifndef FFT_TEST_HELPERS_HPP
#define FFT_TEST_HELPERS_HPP

#include <cmath>
#include <vector>
#include <complex>

static constexpr double EPS = 1e-9;

inline bool approx_eq(double a, double b, double tol = EPS) {
    return std::abs(a - b) < tol;
}

inline bool approx_eq(const std::complex<double>& a,
                      const std::complex<double>& b,
                      double tol = EPS) {
    return approx_eq(a.real(), b.real(), tol)
        && approx_eq(a.imag(), b.imag(), tol);
}

inline bool approx_eq(const std::vector<std::complex<double>>& A,
                      const std::vector<std::complex<double>>& B,
                      double tol = EPS) {
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); ++i)
        if (!approx_eq(A[i], B[i], tol)) return false;
    return true;
}

inline bool approx_eq(const std::vector<std::vector<double>>& A,
                      const std::vector<std::vector<double>>& B,
                      double tol = EPS) {
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i].size() != B[i].size()) return false;
        for (size_t j = 0; j < A[i].size(); ++j)
            if (!approx_eq(A[i][j], B[i][j], tol)) return false;
    }
    return true;
}

#endif // FFT_TEST_HELPERS_HPP
