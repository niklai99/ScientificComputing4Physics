#include "vector_sum_cpp.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <cmath>
#include <numeric>  // For std::accumulate
#include <algorithm>  // For std::min_element and std::max_element

/**
 * @brief Runs the vector sum computation test.
 *
 * This function creates two vectors of size n (with constant values),
 * measures the execution time of the vector sum computation over n_iter iterations,
 * and checks that each element of the resulting vector equals the expected value within a small tolerance.
 *
 * @param n The size of the vectors.
 * @param n_iter The number of iterations for timing measurement (default is 1).
 */
void run_vector_sum_test(std::size_t n, int n_iter = 1) {
    const double a = 3;
    const double x_val = 0.1;
    const double y_val = 7.1;
    const double expected_value = 7.4;
    const double tolerance = 1e-9;

    // Create the input vectors.
    std::vector<double> x_vec(n, x_val);
    std::vector<double> y_vec(n, y_val);
    // Pre-allocate the output vector to be reused.
    std::vector<double> d;
    
    std::vector<double> times;
    times.reserve(n_iter);

    for (int iter = 0; iter < n_iter; ++iter) {
        auto start = std::chrono::high_resolution_clock::now();
        vector_sum(a, x_vec, y_vec, d);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());

        // Verify that all elements match the expected value.
        for (std::size_t i = 0; i < d.size(); ++i) {
            assert(std::fabs(d[i] - expected_value) < tolerance && "Element does not match expected value.");
        }
    }

    // Compute average time.
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double avg_time = sum / n_iter;

    // Compute RMS.
    double sq_sum = 0.0;
    for (double t : times) {
        sq_sum += (t - avg_time) * (t - avg_time);
    }
    double rms_time = std::sqrt(sq_sum / n_iter);

    // Find minimum and maximum times.
    double min_time = *std::min_element(times.begin(), times.end());
    double max_time = *std::max_element(times.begin(), times.end());

    std::cout << "Test passed for n = " << n << " over " << n_iter << " iterations:" << std::endl;
    std::cout << "  Average time: " << avg_time << " seconds" << std::endl;
    std::cout << "  RMS: " << rms_time << " seconds" << std::endl;
    std::cout << "  Min time: " << min_time << " seconds" << std::endl;
    std::cout << "  Max time: " << max_time << " seconds" << std::endl;
}

int main() {
    // Run tests for three different vector sizes with 5 iterations each.
    run_vector_sum_test(10, 100);
    run_vector_sum_test(1000000, 100);   // 10^6
    run_vector_sum_test(100000000, 10); // 10^8
    return 0;
}
