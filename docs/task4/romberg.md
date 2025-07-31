# Romberg integration

This page explains the Romberg integration method implemented in `Integrator::integrateRomberg()`, which uses Richardson extrapolation on trapezoidal estimates to achieve high accuracy.

All code is found in `include/Integrator.hpp`.

## Method outline 

Romberg integration combines the trapezoidal rule with Richardson extrapolation to improve accuracy. It builds a table of estimates, refining them iteratively:

$$
R_{k,0}=T(h_k)
$$

with $h_k=(b-a)/2^k$ and $T(h_k)$ being the trapezoidal estimate with step size $h_k$. Extrapolation then gives:

$$
R_{k,j} = R_{k,j-1} + \frac{R_{k,j-1} - R_{k-1,j-1}}{4^j - 1}
$$

for $j=1,2,\ldots,k$.

## Implementation details

The implementation in `Integrator::integrateRomberg()` constructs the Romberg table and performs extrapolation:

```cpp
// Romberg integration to desired depth 'm'.
double integrateRomberg(double a, double b, int maxIter = 5, double tol = 1e-12) const {
    // Allocate a Romberg table R with dimensions maxIter x maxIter.
    std::vector<std::vector<double>> R(maxIter, std::vector<double>(maxIter, 0.0));

    // Initial trapezoidal rule: use 2^0 segments => n = 2 points.
    int n = 2;
    R[0][0] = integrateTrapz(a, b, n);

    // Build the Romberg table.
    for (int i = 1; i < maxIter; ++i) {
        // Use 2^i segments, which means n = 2^i + 1 sampling points.
        n = (1 << i) + 1;  // 1<<i computes 2^i.
        R[i][0] = integrateTrapz(a, b, n);
        // Apply Richardson extrapolation.
        for (int j = 1; j <= i; ++j) {
            double factor = std::pow(4.0, j);
            R[i][j] = R[i][j-1] + (R[i][j-1] - R[i-1][j-1]) / (factor - 1.0);
        }
        // Check for convergence between the last diagonal elements.
        if (std::fabs(R[i][i] - R[i-1][i-1]) < tol) {
            return R[i][i];
        }
    }
    return R[maxIter-1][maxIter-1];
}
```