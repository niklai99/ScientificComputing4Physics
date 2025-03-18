# Matrix Multiplication

This page demonstrates the matrix multiplication operation in both Python and C++. The function computes

$$
C = A \times B \quad \text{with} \quad C_{ij} = \sum_{k=1}^{N} a_{ik} \, b_{kj},
$$

for constant matrices where every element of \(A\) equals 3 and every element of \(B\) equals the expected value. Hence, each element of \(C\) is expected to be:

$$
n \times (3 \times 7.1) = n \times 21.3.
$$

Test suites measure execution time and verify that the resulting matrix is correct.

---

## Python implementation

```python linenums="1" title="matrix_mult_python.py"
import numpy as np

def matrix_mult(A: np.ndarray, B: np.ndarray) -> np.ndarray:
    """
    Compute the matrix multiplication C = A @ B.

    Parameters:
        A (np.ndarray): A matrix of shape (m, n).
        B (np.ndarray): A matrix of shape (n, p).

    Returns:
        np.ndarray: The resulting matrix C of shape (m, p).

    Raises:
        ValueError: If the inner dimensions of A and B do not match.
    """
    if A.shape[1] != B.shape[0]:
        raise ValueError("Inner dimensions of A and B must match.")
    return A @ B
```

```python linenums="1" title="test_matrix_mult_python.py"
import time
import numpy as np
from matrix_mult_python import matrix_mult

def run_matrix_mult_test(n: int):
    """
    Run the matrix multiplication for n x n matrices, measure the execution time,
    and verify that each element of C equals n * (a_val * b_val).

    Parameters:
        n (int): The dimension of the square matrices.
    """
    a_val = 3.0
    b_val = 7.1
    expected_value = n * (a_val * b_val)  # Each element should equal n * 21.3.
    tolerance = 1e-9

    # Create constant matrices A and B of size (n, n)
    A = np.full((n, n), a_val)
    B = np.full((n, n), b_val)

    start_time = time.perf_counter()
    C = matrix_mult(A, B)
    elapsed_time = time.perf_counter() - start_time

    if not np.allclose(C, expected_value, atol=tolerance):
        raise AssertionError(f"Test failed for n = {n}")

    print(f"Test passed for n = {n} in {elapsed_time:.6f} seconds.")

def main():
    run_matrix_mult_test(10)
    run_matrix_mult_test(100)
    run_matrix_mult_test(10000)

if __name__ == '__main__':
    main()
```

### Run the Python tests

```bash
python test_matrix_mult_python.py
```

### Results

asd

## C++ implementation

```cpp linenums="1" title="matrix_mult_cpp.cpp"
#ifndef MATRIX_MULT_CPP_HPP
#define MATRIX_MULT_CPP_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>

/**
 * @brief Compute the matrix multiplication C = A * B.
 *
 * Given two matrices A and B, where A is of size m x n and B is of size n x p,
 * this function computes the matrix product C of size m x p.
 *
 * @param A The left matrix.
 * @param B The right matrix.
 * @return std::vector<std::vector<double>> The resulting matrix C.
 * @throws std::invalid_argument if the number of columns in A does not equal the number of rows in B.
 */
inline std::vector<std::vector<double>> matrix_mult(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B)
{
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Matrices must not be empty.");
    }
    std::size_t m = A.size();
    std::size_t n = A[0].size();
    std::size_t p = B[0].size();

    if (B.size() != n) {
        throw std::invalid_argument("The number of columns in A must equal the number of rows in B.");
    }
    
    // Initialize C with zeros.
    std::vector<std::vector<double>> C(m, std::vector<double>(p, 0.0));
    
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < p; ++j) {
            double sum = 0.0;
            for (std::size_t k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

#endif // MATRIX_MULT_CPP_HPP
```

```cpp linenums="1" title="test_matrix_mult_cpp.cpp"
#include "matrix_mult_cpp.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <cmath>

/**
 * @brief Run the matrix multiplication test.
 *
 * This function creates two constant n x n matrices A and B, measures the execution time
 * of their multiplication, and verifies that each element of the resulting matrix C equals n * (a_val * b_val)
 * within a small tolerance.
 *
 * @param n The dimension of the square matrices.
 */
void run_matrix_mult_test(std::size_t n) {
    const double a_val = 3.0;
    const double b_val = 7.1;
    const double expected_value = n * (a_val * b_val);  // Each element should equal n * 21.3.
    const double tolerance = 1e-9;

    // Create constant matrices A and B of size n x n.
    std::vector<std::vector<double>> A(n, std::vector<double>(n, a_val));
    std::vector<std::vector<double>> B(n, std::vector<double>(n, b_val));

    auto start = std::chrono::high_resolution_clock::now();
    auto C = matrix_mult(A, B);
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
```

### Compile and run the C++ tests

1. Compile the C++ code:

    ```bash
    g++ -std=c++11 test_matrix_mult_cpp.cpp -o test_matrix_mult_cpp
    ```

2. Run the executable:

    ```bash
    ./test_matrix_mult_cpp
    ```

### Results

asd