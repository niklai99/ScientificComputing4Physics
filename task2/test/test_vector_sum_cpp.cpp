#include "vector_sum_cpp.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <cmath>

/**
 * @brief Runs the vector sum computation test.
 *
 * This function computes the vector sum for a given vector size, measures the execution time,
 * and checks that each element in the resulting vector equals the expected value (7.4) within a small tolerance.
 *
 * @param n The size of the vectors.
 */
void run_vector_sum_test(std::size_t n) {
    const double a = 3;
    const double x_val = 0.1;
    const double y_val = 7.1;
    const double expected_value = 7.4;
    const double tolerance = 1e-9;

    // Start time measurement.
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> d = vector_sum(a, x_val, y_val, n);
    // End time measurement.
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Verify that all elements are as expected.
    for (std::size_t i = 0; i < d.size(); ++i) {
        assert(std::fabs(d[i] - expected_value) < tolerance && "Element does not match expected value.");
    }

    std::cout << "Test passed for N = " << n << " in " << elapsed.count() << " seconds." << std::endl;
}

int main() {
    // Run tests for three vector sizes: 10, 10^6, and 10^8.
    run_vector_sum_test(10);
    run_vector_sum_test(1000000);   // 10^6
    run_vector_sum_test(100000000); // 10^8
    return 0;
}
