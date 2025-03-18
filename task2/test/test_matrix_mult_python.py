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
