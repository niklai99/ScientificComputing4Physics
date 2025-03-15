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
