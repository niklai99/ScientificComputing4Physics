# Comparison with Python

This page documents the Python script `compute_integral.py`, which re-runs numerical integrations on data generated by the C++ program and compares those results against the C++ outputs.

## Numerical integration in Python

The script imports:

```python
from scipy.interpolate import interp1d
from scipy.integrate import trapezoid, simpson
```

The trapezoidal rule is implemented using `scipy.integrate.trapezoid`, and Simpson's rule is implemented using `scipy.integrate.simpson`. Instead, Romberg integration is not directly available in SciPy, so we implement it manually.

### Romberg integration in Python

The Romberg integration method is implemented in Python as follows:

```python
# ============================
# Romberg integration function
# ============================
def romberg(f, a, b, tol=ROMBERG_TOL, rtol=ROMBERG_RTOL, divmax=ROMBERG_DIVMAX):
    """
    Compute the Romberg integration of function f on the interval [a, b]
    using the trapezoidal rule and Richardson extrapolation.
    """
    R = []  # Romberg table: a list of lists
    R.append([(b - a) * (f(a) + f(b)) / 2.0])
    for i in range(1, divmax):
        n_intervals = 2**i
        h = (b - a) / n_intervals
        sum_ = 0.0
        for k in range(1, n_intervals, 2):
            sum_ += f(a + k * h)
        R0 = 0.5 * R[i-1][0] + h * sum_
        row = [R0]
        for j in range(1, i+1):
            factor = 4**j
            extrap = row[j-1] + (row[j-1] - R[i-1][j-1]) / (factor - 1.0)
            row.append(extrap)
        R.append(row)
        if i > 0 and abs(R[i][i] - R[i-1][i-1]) < tol:
            return R[i][i]
    return R[-1][-1]
```


## Comparison with C++ results

The script reads the sampled data from the C++ output files and computes the integrals using the same methods as in C++. The results are then compared:

```python
# Load sampled data from C++ output files
data = np.loadtxt(cpp_data_file)
```

The script then computes the integrals:

```python
N_AB = int(round((B - A) / dx)) + 1
integral_trapz = trapezoid(fx_subset, x_subset)
integral_simpson = simpson(fx_subset, x_subset)
f_interp = interp1d(x_subset, fx_subset, kind='cubic', fill_value="extrapolate")
integral_romberg = romberg(f_interp, A, B, tol=ROMBERG_TOL, rtol=ROMBERG_RTOL, divmax=ROMBERG_DIVMAX)
```

Finally, the script computes the absolute errors between C++ and Python results:

```python
# Compute absolute differences between Python and C++ results
abs_diff_trapz = abs(integral_trapz - cpp_integral_trapz)
abs_diff_simpson = abs(integral_simpson - cpp_integral_simpson)
abs_diff_romberg = abs(integral_romberg - cpp_integral_romberg)
```