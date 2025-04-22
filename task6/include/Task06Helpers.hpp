// include/Task06Helpers.hpp
#ifndef TASK06_HELPERS_HPP
#define TASK06_HELPERS_HPP

#include <vector>
#include <complex>
#include <random>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

using cd = std::complex<double>;

/** Bundles absolute & relative RMSE/MedRSE. */
struct ErrorStats {
    double rmse_abs;
    double medrse_abs;
    double rmse_rel;
    double medrse_rel;
};

/**
 * @brief Generate an M×N matrix of Gaussian random values.
 */
inline std::vector<std::vector<double>>
generate_gaussian_matrix(size_t M,
                         size_t N,
                         double mean,
                         double stddev)
{
    std::mt19937_64 gen{std::random_device{}()};
    std::normal_distribution<double> dist{mean, stddev};
    std::vector<std::vector<double>> A(M, std::vector<double>(N));
    for (auto& row : A)
        for (auto& x : row)
            x = dist(gen);
    return A;
}

/**
 * @brief Promote a real matrix to complex (imag=0).
 */
inline std::vector<std::vector<cd>>
promote_to_complex(const std::vector<std::vector<double>>& A)
{
    size_t M = A.size();
    size_t N = M ? A[0].size() : 0;
    std::vector<std::vector<cd>> out(M, std::vector<cd>(N));
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            out[i][j] = { A[i][j], 0.0 };
    return out;
}

// internal helper: RMSE & median‐of‐squares
static inline void compute_two_stats(const std::vector<double>& e,
                                     double& out_rmse,
                                     double& out_medrse)
{
    size_t N = e.size();
    std::vector<double> sq(N);
    for (size_t i = 0; i < N; ++i) sq[i] = e[i]*e[i];

    double mean_sq = std::accumulate(sq.begin(), sq.end(), 0.0)/double(N);
    out_rmse = std::sqrt(mean_sq);

    auto mid = sq.begin() + N/2;
    std::nth_element(sq.begin(), mid, sq.end());
    double med = *mid;
    if (N%2==0) {
        double lo = *std::max_element(sq.begin(), mid);
        med = 0.5*(med + lo);
    }
    out_medrse = std::sqrt(med);
}

/**
 * @brief Errors for complex→complex roundtrip.
 */
inline ErrorStats evaluate_c2c_roundtrip(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<cd>>&     Arec)
{
    size_t M = A.size(), N = A[0].size();
    std::vector<double> abs_err; abs_err.reserve(M*N);
    std::vector<double> rel_err; rel_err.reserve(M*N);

    for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < N; ++j) {
        double orig = A[i][j];
        double rec  = Arec[i][j].real();
        double e    = rec - orig;
        abs_err.push_back(e);
        rel_err.push_back(e/orig);
    }

    ErrorStats s;
    compute_two_stats(abs_err, s.rmse_abs,  s.medrse_abs);
    compute_two_stats(rel_err, s.rmse_rel,  s.medrse_rel);
    return s;
}

/**
 * @brief Errors for real→complex→real roundtrip.
 */
inline ErrorStats evaluate_r2c_roundtrip(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& Arec)
{
    size_t M = A.size(), N = A[0].size();
    std::vector<double> abs_err; abs_err.reserve(M*N);
    std::vector<double> rel_err; rel_err.reserve(M*N);

    for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < N; ++j) {
        double orig = A[i][j];
        double rec  = Arec[i][j];
        double e    = rec - orig;
        abs_err.push_back(e);
        rel_err.push_back(e/orig);
    }

    ErrorStats s;
    compute_two_stats(abs_err, s.rmse_abs,  s.medrse_abs);
    compute_two_stats(rel_err, s.rmse_rel,  s.medrse_rel);
    return s;
}

/**
 * @brief Print a titled summary of error‐statistics.
 */
inline void print_error_stats(const std::string& title,
                              const ErrorStats&  s)
{
    std::cout << "=== " << title << " ===\n"
              << "  RMSE(abs) = "   << s.rmse_abs   << "\n"
              << "  MedRSE(abs)= "  << s.medrse_abs << "\n"
              << "  RMSE(rel) = "   << s.rmse_rel   << "\n"
              << "  MedRSE(rel)= "  << s.medrse_rel << "\n\n";
}

/**
 * @brief Compute the “DC term” (sum over all entries) of any 2D matrix.
 */
template<typename T>
inline T compute_dc_term(const std::vector<std::vector<T>>& Mtx)
{
    T sum{0};
    for (auto& row : Mtx)
        for (auto& x : row)
            sum += x;
    return sum;
}

#endif // TASK06_HELPERS_HPP
