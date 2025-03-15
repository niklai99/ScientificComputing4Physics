# Matrix Multiplication Implementation and Testing

This page demonstrates the matrix multiplication operation in both Python and C++. The function computes

\[
C = A \times B \quad \text{with} \quad C_{ij} = \sum_{k=1}^{N} a_{ik} \, b_{kj},
\]

for constant matrices where every element of \(A\) equals 3 and every element of \(B\) equals 7.1. Hence, each element of \(C\) is expected to be:
\[
n \times (3 \times 7.1) = n \times 21.3.
\]

Test suites measure execution time and verify that the resulting matrix is correct.

---

## Python Implementation

```python linenums="1" title="matrix_mult_python.py"
import numpy as np

def matrix_mult(n: int, a_val: float, b_val: float) -> np.ndarray:
    """
    Compute the matrix multiplication C = A * B for constant matrices.

    Each matrix A and B is an n x n matrix where every element of A is set to a_val and
    every element of B is set to b_val. The standard definition is:
        C[i, j] = sum_{k=0}^{n-1} A[i, k] * B[k, j].

    Since A and B are constant, every element of C equals:
        n * (a_val * b_val).

    Parameters:
        n (int): The dimension of the square matrices.
        a_val (float): The constant value for matrix A.
        b_val (float): The constant value for matrix B.

    Returns:
        np.ndarray: The resulting n x n matrix C.
    """
    A = np.full((n, n), a_val)
    B = np.full((n, n), b_val)
    C = A @ B  # Using numpy's efficient matrix multiplication.
    return C
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
        n (int): The dimension of the matrices.
    """
    a_val = 3.0
    b_val = 7.1
    expected_value = n * (a_val * b_val)  # Each element should equal n * 21.3.
    tolerance = 1e-9

    start_time = time.perf_counter()
    C = matrix_mult(n, a_val, b_val)
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

## C++ Implementation

```cpp linenums="1" title="matrix_mult_cpp.cpp"
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