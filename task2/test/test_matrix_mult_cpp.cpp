#include "matrix_mult_cpp.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <cmath>

/**
 * @brief Run the matrix multiplication test.
 *
 * Computes C = A * B for matrices of dimension n x n, measures the execution time,
 * and verifies that each element of C equals n * (a_val * b_val) within a small tolerance.
 *
 * @param n The dimension of the matrices.
 */
void run_matrix_mult_test(std::size_t n) {
    const double a_val = 3.0;
    const double b_val = 7.1;
    const double expected_value = n * (a_val * b_val);  // Expected value: n * 21.3.
    const double tolerance = 1e-9;

    auto start = std::chrono::high_resolution_clock::now();
    auto C = matrix_mult(n, a_val, b_val);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    for (std::size_t i = 0; i < C.size(); ++i) {
        for (std::size_t j = 0; j < C[i].size(); ++j) {
            assert(std::fabs(C[i][j] - expected_value) < tolerance && "Element does not match expected value.");
        }
    }
    std::cout << "Test passed for n = " << n << " in " << elapsed.count() << " seconds." << std::endl;
}

int main() {
    run_matrix_mult_test(10);
    run_matrix_mult_test(100);
    run_matrix_mult_test(10000);
    return 0;
}
