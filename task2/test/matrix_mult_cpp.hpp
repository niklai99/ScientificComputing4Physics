#ifndef MATRIX_MULT_CPP_HPP
#define MATRIX_MULT_CPP_HPP

#include <vector>
#include <cstddef>

/**
 * @brief Compute the matrix multiplication C = A * B for constant matrices.
 *
 * Both A and B are n x n matrices where every element of A is a_val and every element
 * of B is b_val. The standard multiplication is given by:
 *      C[i][j] = sum_{k=0}^{n-1} A[i][k] * B[k][j].
 *
 * Since every element of A is a_val and every element of B is b_val, each element of C
 * is equal to n * (a_val * b_val).
 *
 * @param n The dimension of the square matrices.
 * @param a_val The constant value for matrix A.
 * @param b_val The constant value for matrix B.
 * @return std::vector<std::vector<double>> The resulting matrix C.
 */
inline std::vector<std::vector<double>> matrix_mult(std::size_t n, double a_val, double b_val) {
    std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));
    double product = a_val * b_val;
    double value = n * product; // Each element should be n * (a_val * b_val).
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            C[i][j] = value;
        }
    }
    return C;
}

#endif // MATRIX_MULT_CPP_HPP
