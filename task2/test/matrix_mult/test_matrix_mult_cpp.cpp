#include "matrix_mult_cpp.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <cmath>
#include <numeric>    // For std::accumulate
#include <algorithm>  // For std::min_element and std::max_element

/**
 * @brief Run the matrix multiplication test.
 *
 * This function creates two constant n x n matrices A and B (stored as contiguous vectors),
 * measures the execution time of their multiplication over n_iter iterations, and verifies that
 * each element of the resulting matrix equals n * (a_val * b_val) within a small tolerance.
 *
 * @param n The dimension of the square matrices.
 * @param n_iter The number of iterations for timing measurement.
 */
void run_matrix_mult_test(std::size_t n, int n_iter = 1) {
    const double a_val = 3.0;
    const double b_val = 7.1;
    const double expected_value = n * (a_val * b_val);  // Expected value: n * (a_val * b_val)
    const double tolerance = 1e-7;
    
    // Create matrices A and B as contiguous vectors in row-major order.
    std::vector<double> A(n * n, a_val);
    std::vector<double> B(n * n, b_val);
    
    // Pre-allocate the output matrix to be reused.
    std::vector<double> C; 
    std::vector<double> times;
    times.reserve(n_iter);
    
    for (int iter = 0; iter < n_iter; ++iter) {
        auto start = std::chrono::high_resolution_clock::now();
        matrix_mult(A, B, C, n, n, n);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
        
        // Verify that each element in C is as expected.
        for (const auto& v : C) {
            assert(std::fabs(v - expected_value) < tolerance && "Element does not match expected value.");
        }
    }
    
    // Compute statistics: average, RMS, minimum, and maximum times.
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double avg_time = sum / n_iter;
    
    double sq_sum = 0.0;
    for (double t : times) {
        sq_sum += (t - avg_time) * (t - avg_time);
    }
    double rms_time = std::sqrt(sq_sum / n_iter);
    
    double min_time = *std::min_element(times.begin(), times.end());
    double max_time = *std::max_element(times.begin(), times.end());
    
    std::cout << "Test passed for n = " << n << " over " << n_iter << " iterations:" << std::endl;
    std::cout << "  Average time: " << avg_time << " seconds" << std::endl;
    std::cout << "  RMS: " << rms_time << " seconds" << std::endl;
    std::cout << "  Min time: " << min_time << " seconds" << std::endl;
    std::cout << "  Max time: " << max_time << " seconds" << std::endl;
}

int main() {
    // Run tests for different matrix sizes.
    run_matrix_mult_test(10, 100000);
    run_matrix_mult_test(100, 100000);
    // run_matrix_mult_test(10000, 5);
    return 0;
}