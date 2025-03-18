import time
import math
import numpy as np
from matrix_mult_python import matrix_mult

def run_matrix_mult_test(n: int, n_iter: int = 1):
    """
    Run the matrix multiplication for n x n matrices, measure the execution time
    over n_iter iterations, and verify that each element of C equals n * (a_val * b_val).

    Parameters:
        n (int): The dimension of the square matrices.
        n_iter (int): The number of iterations for timing measurement (default is 1).

    Raises:
        AssertionError: If any element of the computed matrix does not match the expected value.
    """
    a_val = 3.0
    b_val = 7.1
    expected_value = n * (a_val * b_val)  # Each element should equal n * (a_val * b_val).
    tolerance = 1e-9

    # Create constant matrices A and B of size (n, n)
    A = np.full((n, n), a_val)
    B = np.full((n, n), b_val)
    
    times = []
    for i in range(n_iter):
        start_time = time.perf_counter()
        C = matrix_mult(A, B)
        elapsed_time = time.perf_counter() - start_time
        times.append(elapsed_time)
        
        if not np.allclose(C, expected_value, atol=tolerance):
            raise AssertionError(f"Test failed for n = {n} on iteration {i+1}")
    
    avg_time = sum(times) / n_iter
    rms_time = math.sqrt(sum((t - avg_time) ** 2 for t in times) / n_iter)
    min_time = min(times)
    max_time = max(times)
    
    print(f"Test passed for n = {n} over {n_iter} iterations:")
    print(f"  Average time: {avg_time:.2e} seconds")
    print(f"  RMS: {rms_time:.2e} seconds")
    print(f"  Min time: {min_time:.2e} seconds")
    print(f"  Max time: {max_time:.2e} seconds")

def main():
    run_matrix_mult_test(10,  n_iter=100000)
    run_matrix_mult_test(100, n_iter=100000)
    # run_matrix_mult_test(10000, n_iter=10)

if __name__ == '__main__':
    main()
