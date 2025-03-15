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
