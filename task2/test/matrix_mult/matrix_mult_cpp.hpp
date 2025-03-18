#ifndef MATRIX_MULT_CPP_HPP
#define MATRIX_MULT_CPP_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <cassert>

/**
 * @brief Compute the matrix multiplication C = A * B.
 *
 * Matrices are represented as a single contiguous vector in row-major order.
 * Given matrix A of dimensions m x n and matrix B of dimensions n x p,
 * this function computes the product C (of dimensions m x p) and stores it in C_out.
 * The output vector C_out is resized accordingly.
 *
 * @param A The left matrix (m x n), stored in row-major order.
 * @param B The right matrix (n x p), stored in row-major order.
 * @param C_out Output matrix (m x p), stored in row-major order.
 * @param m Number of rows in A.
 * @param n Number of columns in A (and rows in B).
 * @param p Number of columns in B.
 * @throws std::invalid_argument if the dimensions of A or B are inconsistent.
 */
inline void matrix_mult(const std::vector<double>& A,
                        const std::vector<double>& B,
                        std::vector<double>& C_out,
                        std::size_t m,
                        std::size_t n,
                        std::size_t p)
{
    if (A.size() != m * n || B.size() != n * p) {
        throw std::invalid_argument("Matrix dimensions do not match the provided sizes.");
    }
    
    // Resize output vector to hold the result (m x p) and initialize with zeros.
    C_out.assign(m * p, 0.0);
    
    // Use loop reordering to improve cache locality.
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t k = 0; k < n; ++k) {
            double a_ik = A[i * n + k];
            for (std::size_t j = 0; j < p; ++j) {
                C_out[i * p + j] += a_ik * B[k * p + j];
            }
        }
    }
}

#endif // MATRIX_MULT_CPP_HPP