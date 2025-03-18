#ifndef MATRIX_MULT_CPP_HPP
#define MATRIX_MULT_CPP_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>

/**
 * @brief Compute the matrix multiplication C = A * B.
 *
 * Given two matrices A and B, where A is of size m x n and B is of size n x p,
 * this function computes the matrix product C of size m x p.
 *
 * @param A The left matrix.
 * @param B The right matrix.
 * @return std::vector<std::vector<double>> The resulting matrix C.
 * @throws std::invalid_argument if the number of columns in A does not equal the number of rows in B.
 */
inline std::vector<std::vector<double>> matrix_mult(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B)
{
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Matrices must not be empty.");
    }
    std::size_t m = A.size();
    std::size_t n = A[0].size();
    std::size_t p = B[0].size();

    if (B.size() != n) {
        throw std::invalid_argument("The number of columns in A must equal the number of rows in B.");
    }
    
    // Initialize C with zeros.
    std::vector<std::vector<double>> C(m, std::vector<double>(p, 0.0));
    
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < p; ++j) {
            double sum = 0.0;
            for (std::size_t k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

#endif // MATRIX_MULT_CPP_HPP
