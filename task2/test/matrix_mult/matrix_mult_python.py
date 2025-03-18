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
