#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP

#include <iostream>
#include <chrono>
#include <cassert>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <vector>
#include <string>

#include "VectorGenerator.hpp"
#include "VectorSumInterface.hpp"

/**
 * @brief Validates that the computed DAXPY result vector follows the expected Gaussian distribution.
 *
 * This function checks whether the result vector @p d, computed as d = a * x + y, conforms to
 * a Gaussian distribution with mean 0 and RMS (root mean square) equal to sqrt(a²+1).
 *
 * It calculates the sample mean and RMS from the vector and compares these to the theoretical values.
 *
 * @param d The result vector to be validated.
 * @param a The scalar multiplier used in the DAXPY computation.
 * @param tol_mean Tolerance allowed for the difference in means.
 * @param tol_rms Tolerance allowed for the difference in RMS values.
 * @return true if both the sample mean and RMS are within the tolerances; false otherwise.
 */
bool validate_gaussian_daxpy_result(const std::vector<double>& d, double a,
                                    double tol_mean, double tol_rms) {
    std::size_t n = d.size();
    
    // Calculate the sample mean.
    double sum = std::accumulate(d.begin(), d.end(), 0.0);
    double mean = sum / n;
    
    // Calculate the sample RMS (root mean square).
    double sq_sum = std::inner_product(d.begin(), d.end(), d.begin(), 0.0);
    double sample_rms = std::sqrt(sq_sum / n);
    
    // Theoretical values for a Gaussian distribution N(0, a²+1)
    double expected_mean = 0.0;
    double expected_rms = std::sqrt(a * a + 1.0);
    
    // Check if the calculated mean and RMS are within the specified tolerances.
    bool mean_ok = std::fabs(mean - expected_mean) < tol_mean;
    bool rms_ok  = std::fabs(sample_rms - expected_rms) < tol_rms;
    
    // If the validation fails, output diagnostic details.
    if (!mean_ok || !rms_ok) {
        std::cerr << "Validation failed:\n"
                  << "  Sample mean = " << mean << " (expected " << expected_mean << ")\n"
                  << "  Sample RMS  = " << sample_rms << " (expected " << expected_rms << ")\n";
    }
    
    return mean_ok && rms_ok;
}

/**
 * @brief Runs the DAXPY test using the provided vector summation implementation.
 *
 * This function generates two input vectors @p x and @p y using Gaussian random numbers (mean 0, stddev 1),
 * then computes the result vector @p d as d = a * x + y using the specified summator.
 *
 * It validates that the output vector @p d is consistent with a Gaussian distribution N(0, a²+1)
 * by checking the sample mean and RMS against theoretical values. Additionally, it measures the
 * execution time over multiple iterations and outputs performance statistics.
 *
 * @param n The size of the vectors.
 * @param a The scalar multiplier used in the DAXPY operation.
 * @param n_iter The number of iterations for timing measurements.
 * @param summator The vector summation implementation to use.
 * @param test_name A descriptive name for the test (e.g., "Default Implementation").
 */
void run_vector_sum_test(std::size_t n, double a, int n_iter,
                         VectorSumInterface& summator,
                         const std::string& test_name) {
    // Dynamic tolerances based on statistical error.
    // For a Gaussian distribution N(0, a²+1), the standard deviation is sigma = sqrt(a²+1).
    // The standard error on the mean is sigma/sqrt(n)
    // The standard error on the RMS is sigma/sqrt(2*n)
    // So we set tolerances as:
    const double sigma = std::sqrt(a * a + 1.0);
    const double tol_factor = 3.2;
    double tol_mean = tol_factor * sigma / std::sqrt(n);
    double tol_rms  = tol_factor * sigma / std::sqrt(n*2);
    
    // Expected theoretical values.
    const double expected_mean = 0.0;
    const double expected_rms = sigma;
    
    // Generate input vectors x and y with Gaussian random values.
    std::vector<double> x = VectorGenerator::generate_gaussian_vector(n);
    std::vector<double> y = VectorGenerator::generate_gaussian_vector(n);
    std::vector<double> d;  // This vector will hold the computed result d = a * x + y.
    
    // Vectors to store the execution times and errors for each iteration.
    std::vector<double> times;
    times.reserve(n_iter);
    double sum_error_mean = 0.0;
    double sum_error_rms  = 0.0;
    
    // Execute the test for n_iter iterations.
    for (int iter = 0; iter < n_iter; ++iter) {
        auto start = std::chrono::high_resolution_clock::now();
        // Perform the DAXPY computation.
        summator.compute_sum(x, y, a, d);
        auto end = std::chrono::high_resolution_clock::now();
        
        // Record the elapsed time in seconds.
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());
        
        // Compute sample mean.
        double computed_mean = std::accumulate(d.begin(), d.end(), 0.0) / n;
        // Compute sample RMS.
        double computed_sq_sum = std::inner_product(d.begin(), d.end(), d.begin(), 0.0);
        double computed_rms = std::sqrt(computed_sq_sum / n);
        
        // Compute absolute errors for mean and RMS.
        double error_mean = std::fabs(computed_mean - expected_mean);
        double error_rms = std::fabs(computed_rms - expected_rms);
        sum_error_mean += error_mean;
        sum_error_rms  += error_rms;
        
        // Validate that the resulting vector d has the expected Gaussian properties.
        bool valid = validate_gaussian_daxpy_result(d, a, tol_mean, tol_rms);
        assert(valid && "DAXPY result does not match the expected Gaussian distribution!");
    }
    
    // Calculate performance statistics: average time, RMS time, minimum, and maximum times.
    double total_time = std::accumulate(times.begin(), times.end(), 0.0);
    double avg_time = total_time / n_iter;
    
    double sq_sum = 0.0;
    for (double t : times) {
        sq_sum += (t - avg_time) * (t - avg_time);
    }
    double rms_time = std::sqrt(sq_sum / n_iter);
    
    // double min_time = *std::min_element(times.begin(), times.end());
    // double max_time = *std::max_element(times.begin(), times.end());
    
    // Calculate the average errors over all iterations.
    double avg_error_mean = sum_error_mean / n_iter;
    double avg_error_rms = sum_error_rms / n_iter;
    
    // Output the test results and timing statistics.
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << test_name << " Test:" << std::endl;
    std::cout << "Vector size (n): " << n << ", Scalar (a): " << a 
              << ", Iterations: " << n_iter << std::endl;
    std::cout << "Dynamic tolerances: tol_mean = " << tol_mean 
              << ", tol_rms = " << tol_rms << std::endl;
    std::cout << "Average errors: mean error = " << avg_error_mean 
              << ", RMS error = " << avg_error_rms << std::endl;
    std::cout << "  Average time: " << avg_time << " seconds" << std::endl;
    std::cout << "  RMS time:     " << rms_time << " seconds" << std::endl;
    std::cout << "Test passed successfully." << std::endl;
    std::cout << "------------------------------------------------------" << std::endl << std::endl;
}

#endif // TEST_SUITE_HPP