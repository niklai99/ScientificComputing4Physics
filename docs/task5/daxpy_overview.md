# Summing Vectors with DAXPY

In Task 5b, we extend the vector summation work to the DAXPY operation

$$
d = a \cdot x + y
$$

where both `x` and `y` are filled with independently drawn Gaussian (normal) random numbers with mean 0 and standard deviation 1.

## Random Vector Generation

Random vectors `x` and `y` of length `n` are produced by `VectorGenerator.hpp`. The generator uses the C++11 `<random>` library. A simplified excerpt from `VectorGenerator.hpp` shows the core approach:

```cpp
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
```

## DAXPY Implementations

We use the existing Strategy Pattern from Task 3. Both implementations derive from `VectorSumInterface`.

## Testing DAXPY

The file `TestSuite.hpp` provides a template function `run_vector_sum_test` that executes a sequence of DAXPY operations and collects statistics. A representative snippet from `testDaxpy.cpp` illustrates its use:

```cpp
int main(int argc, char** argv) {
    // Parse command-line: n, a, n_iter
    int n      = 1000000;
    double a   = 3.0;
    int n_iter = 100;

    VectorSumDefault default_sum;
    run_vector_sum_test(n, a, n_iter, default_sum, "default");

    VectorSumGSL gsl_sum;
    run_vector_sum_test(n, a, n_iter, gsl_sum, "gsl");

    return 0;
}
```

Within `run_vector_sum_test`, each iteration:

1. Calls `generate_gaussian_vector` to create random vectors `x` and `y`.
2. Executes the DAXPY operation.
3. Computes the sample mean and standard deviation of the results.
4. Record these statistics over all iterations.

After `n_iter` runs, the suite prints the average mean and RMS values alongside the theoretical expectations:

- Expected mean: \( a \cdot \text{mean}(x) + \text{mean}(y) = 0 \)
- Expected RMS: \( \sqrt{a^2 + 1} \)

We consider the test passed if the average sample mean is close to zero and the average sample standard deviation is close to the expected RMS value. The typical tolerance is on the order of the standard error:

- Mean tolerance: \(\sigma_d / \sqrt{n}\)
- RMS tolerance: \(\sigma_d / \sqrt{2(n-1)}\)

This test ensures that the DAXPY implementations correctly handle random inputs and produce results consistent with theoretical predictions.

## Executing the Tests

To build and run the tests, use the standard build script and then:

```bash
run testDaxpy 1000000 3.0 100
```

This command runs the DAXPY tests with:
- `n = 1000000`: Length of the vectors.
- `a = 3.0`: Scalar multiplier.
- `n_iter = 100`: Number of iterations for averaging results.

The output summarizes each strategy’s statistical performance, confirming correctness and allowing performance comparison.