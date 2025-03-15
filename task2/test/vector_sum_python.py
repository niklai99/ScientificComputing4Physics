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
