import numpy as np

def vector_sum(a: float, x: np.ndarray, y: np.ndarray) -> np.ndarray:
    """
    Compute the vector sum d = a * x + y, where x and y are vectors.

    Parameters:
        a (float): Scalar multiplier.
        x (np.ndarray): A vector.
        y (np.ndarray): A vector.

    Returns:
        np.ndarray: The resulting vector d.

    Raises:
        ValueError: If x and y do not have the same shape.
    """
    if x.shape != y.shape:
        raise ValueError("Vectors x and y must have the same shape.")
    return a * x + y
