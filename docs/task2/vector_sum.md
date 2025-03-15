# Vector Sum Implementation and Testing

This page demonstrates the vector sum operation in both Python and C++. The function computes the vector sum

$$
d = a \cdot x + y,
$$

where \(x\) and \(y\) are vectors of dimension \(N\). The test suites measure the execution time and verify that every element equals 7.4.

---

## Python implementation

```python linenums="1" title="vector_sum_python.py"
import numpy as np

def vector_sum(a: float, x_val: float, y_val: float, n: int) -> np.ndarray:
    """
    Compute the vector sum d = a * x + y, where x and y are vectors of length n.

    Parameters:
        a (float): Scalar multiplier.
        x_val (float): Value to fill vector x.
        y_val (float): Value to fill vector y.
        n (int): Dimension of the vectors.

    Returns:
        np.ndarray: The resulting vector d.
    """
    x = np.full(n, x_val)
    y = np.full(n, y_val)
    return a * x + y
```

```python linenums="1" title="test_vector_sum_python.py"
import time
import numpy as np
from vector_sum_python import vector_sum

def run_vector_sum_test(n: int):
    """
    Run the vector sum computation for a vector of size n, measure the execution time,
    and verify that each element equals the expected value within a defined tolerance.
    
    Parameters:
        n (int): The size of the vectors.
    
    Raises:
        AssertionError: If any element of the computed vector does not match the expected value.
    """
    a = 3
    x_val = 0.1
    y_val = 7.1
    expected_value = 7.4
    tolerance = 1e-9

    # Measure the start time.
    start_time = time.perf_counter()
    d = vector_sum(a, x_val, y_val, n)
    # Measure the elapsed time.
    elapsed_time = time.perf_counter() - start_time

    # Verify that all elements equal the expected value within the tolerance.
    if not np.allclose(d, expected_value, atol=tolerance):
        raise AssertionError(f"Test failed for N = {n}")

    print(f"Test passed for N = {n} in {elapsed_time:.6f} seconds.")

def main():
    run_vector_sum_test(10)
    run_vector_sum_test(10**6)
    run_vector_sum_test(10**8)

if __name__ == '__main__':
    main()
```

### Run the Python tests

Within the container, execute

```bash
python test_vector_sum_python.py
```

### Results

asd

## C++ implementation

```cpp linenums="1" title="vector_sum_cpp.cpp"
#ifndef VECTOR_SUM_CPP_HPP
#define VECTOR_SUM_CPP_HPP

#include <vector>
#include <cstddef>

/**
 * @brief Compute the vector sum d = a * x + y.
 *
 * This function creates two vectors of length n where every element of x is set to x_val and every element
 * of y is set to y_val. It then computes:
 *     d[i] = a * x_val + y_val, for i = 0 to n-1.
 *
 * @param a      Scalar multiplier.
 * @param x_val  Constant value to fill vector x.
 * @param y_val  Constant value to fill vector y.
 * @param n      Dimension of the vectors.
 * @return std::vector<double> The resulting vector d.
 */
inline std::vector<double> vector_sum(double a, double x_val, double y_val, std::size_t n) {
    std::vector<double> d(n);
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = a * x_val + y_val;
    }
    return d;
}

#endif // VECTOR_SUM_CPP_HPP
```

```cpp linenums="1" title="test_vector_sum_cpp.cpp"
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
 * and checks that each element equals the expected value (7.4) within a small tolerance.
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
    run_vector_sum_test(10);
    run_vector_sum_test(1000000);   // 10^6
    run_vector_sum_test(100000000); // 10^8
    return 0;
}
```

### Compile and run the C++ tests

1. Compile the C++ code:

    ```bash
    g++ -std=c++11 test_vector_sum_cpp.cpp -o test_vector_sum_cpp
    ```

2. Run the executable:

    ```bash
    ./test_vector_sum_cpp
    ```

### Results

asd
