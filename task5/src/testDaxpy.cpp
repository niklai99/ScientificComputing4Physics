#include "TestSuite.hpp"
#include "VectorSumDefault.hpp"
#include "VectorSumGSL.hpp"
#include <iostream>
#include <cstdlib>

/**
 * @brief Main entry point for running the DAXPY tests.
 *
 * This program runs tests for the DAXPY operation (d = a * x + y) using two implementations:
 * the default element-wise loop and the GSL-based method. It allows the user to override the default
 * vector size, scalar multiplier, and number of iterations through command-line arguments.
 *
 * Command-line arguments:
 *   argv[1] - Vector size (n)
 *   argv[2] - Scalar multiplier (a)
 *   argv[3] - Number of iterations (n_iter)
 *
 * @return int Exit status.
 */
int main(int argc, char** argv) {
    // Set default parameters.
    std::size_t n = 1000000;  // Default size of the vectors.
    double a = 3.0;           // Default scalar multiplier.
    int n_iter = 100;         // Default number of iterations for timing.
    
    // If command-line arguments are provided, override the defaults.
    if (argc >= 4) {
        n = std::stoul(argv[1]);   // Convert the first argument to vector size.
        a = std::stod(argv[2]);    // Convert the second argument to the scalar multiplier.
        n_iter = std::stoi(argv[3]); // Convert the third argument to the number of iterations.
    }
    
    // Output the test configuration.
    std::cout << "Running DAXPY tests with vector size (n) = " << n 
              << ", scalar (a) = " << a 
              << ", iterations = " << n_iter << std::endl << std::endl;
              
    // Create instances of the two vector summation implementations.
    VectorSumDefault defaultSummator;
    VectorSumGSL gslSummator;
    
    // Execute the test suite for each implementation.
    run_vector_sum_test(n, a, n_iter, defaultSummator, "Default Implementation");
    run_vector_sum_test(n, a, n_iter, gslSummator, "GSL Implementation");
    
    return 0;
}
