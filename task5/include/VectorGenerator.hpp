#ifndef VECTOR_GENERATOR_HPP
#define VECTOR_GENERATOR_HPP

#include <vector>
#include <random>
#include <cstddef>

/**
 * @brief Utility class for generating vectors filled with random numbers.
 *
 * This class provides static methods to generate vectors with values drawn from various
 * random distributions. In particular, the generate_gaussian_vector() method creates a vector
 * with elements sampled from a Gaussian (normal) distribution with a mean of 0 and a standard deviation of 1.
 *
 * Example usage:
 * @code
 *     std::vector<double> my_vector = VectorGenerator::generate_gaussian_vector(100);
 * @endcode
 */
class VectorGenerator {
public:
    /**
     * @brief Generates a vector of Gaussian random numbers.
     *
     * This function returns a vector of size @p N, where each element is randomly generated
     * from a normal distribution with mean 0 and standard deviation 1.
     *
     * @param N The number of elements to generate.
     * @return std::vector<double> A vector of size @p N with Gaussian-distributed values.
     */
    static std::vector<double> generate_gaussian_vector(std::size_t N) {
        std::vector<double> vec(N);
        // Seed the random number generator using a random device.
        static std::random_device rd;
        static std::mt19937 gen(rd());
        // Define the normal distribution with mean 0.0 and standard deviation 1.0.
        std::normal_distribution<double> dist(0.0, 1.0);
        
        // Fill the vector with random numbers sampled from the Gaussian distribution.
        for (std::size_t i = 0; i < N; ++i) {
            vec[i] = dist(gen);
        }
        return vec;
    }
};

#endif // VECTOR_GENERATOR_HPP
