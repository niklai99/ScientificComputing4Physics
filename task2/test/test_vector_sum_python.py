import time
import math
import numpy as np
from vector_sum_python import vector_sum

def run_vector_sum_test(n: int, n_iter: int = 1):
    """
    Run the vector sum computation for vectors of size n, measure the execution time
    over n_iter iterations, and verify that each element equals the expected value
    within a defined tolerance.

    Parameters:
        n (int): The size of the vectors.
        n_iter (int): The number of iterations for timing measurement (default 1).

    Raises:
        AssertionError: If any element of the computed vector does not match the expected value.
    """
    a = 3
    x_val = 0.1
    y_val = 7.1
    expected_value = 7.4
    tolerance = 1e-9

    # Create the input vectors of size n.
    x_vec = np.full(n, x_val)
    y_vec = np.full(n, y_val)
    
    times = []
    
    for i in range(n_iter):
        start_time = time.perf_counter()
        d = vector_sum(a, x_vec, y_vec)
        elapsed_time = time.perf_counter() - start_time
        times.append(elapsed_time)
        
        # Verify that all elements equal the expected value within the tolerance.
        if not np.allclose(d, expected_value, atol=tolerance):
            raise AssertionError(f"Test failed for N = {n} on iteration {i+1}")
    
    # Compute average, RMS, min, and max.
    avg_time = sum(times) / n_iter
    rms_time = math.sqrt(sum((t - avg_time) ** 2 for t in times) / n_iter)
    min_time = min(times)
    max_time = max(times)
    
    print(f"Test passed for N = {n} over {n_iter} iterations:")
    print(f"  Average time: {avg_time:.6f} seconds")
    print(f"  RMS: {rms_time:.6f} seconds")
    print(f"  Min time: {min_time:.6f} seconds")
    print(f"  Max time: {max_time:.6f} seconds")

def main():
    run_vector_sum_test(10, n_iter=100)
    run_vector_sum_test(10**6, n_iter=100)
    run_vector_sum_test(10**8, n_iter=10)

if __name__ == '__main__':
    main()
