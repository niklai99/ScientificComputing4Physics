In this task, we study numerical integration of the function

$$
f(x) = e^{x} \, \cos(x)
$$

over a chosen interval (e.g., $[0, \pi]$). The goal is to compare three classical methods—Trapezoidal rule, Simpson’s rule, and Romberg integration. We first describe the mathematical foundations, then explain how they are implemented in C++, and finally show how a Python script validates and visualizes the results.

## Mathematical background

### Trapezoidal rule

The trapezoidal rule approximates the integral

$$
\int_a^b f(x) \, dx
$$

by subdividing the interval $[a, b]$ into $N-1$ subintervals of equal width $h = (b-a)/(N-1)$, and approximating the area under the curve as a series of trapezoids:

$$
I_{\text{trapz}} = h \left[\frac{1}{2} f(a) + \sum_{i=1}^{N-2} f(a + i h) + \frac{1}{2} f(b) \right]
$$

This method has a global error that scales as $O(h^2)$, where $h$ is the step size.

### Simpson's rule

Simpson’s rule improves accuracy by using parabolic segments. It requires an odd number of sampling points $N$. The integral is approximated as:

$$
I_{\text{simp}} = \frac{h}{3} \left[ f(a) + 4 \sum_{i=1, \text{odd}}^{N-2} f(a + i h) + 2 \sum_{i=2, \text{even}}^{N-3} f(a + i h) + f(b) \right]
$$

This method has a global error that scales as $O(h^4)$, making it significantly more accurate than the trapezoidal rule for the same number of points.ù

### Romberg integration

Romberg integration combines the trapezoidal rule with Richardson extrapolation to achieve high accuracy. It uses repeated trapezoidal estimates with decreasing step sizes $h_k = (b-a)/2^k$ and builds a table of estimates. The Romberg table is constructed as follows:

$$
R_{k,0} = T(h_k)
$$

with $T(h_k)$ being the trapezoidal estimate with step size $h_k$. Extrapolation then gives:

$$
R_{k,j} = R_{k,j-1} + \frac{R_{k,j-1} - R_{k-1,j-1}}{4^j - 1}
$$

for $j=1,2,\ldots,k$.

## C++ Implementation

The C++ program `computeIntegral` executes the following steps:

1. **Sampling**: Generates $N$ evenly spaced points $x_i$ in $[a, b]$ and evaluates $f(x_i)$.
2. **Integration**: Applies the three numerical integration methods (trapezoidal, Simpson's, and Romberg) to compute the integral over the specified interval.
3. **Output**: Writes the sampled points and integration results to files.

## Comparison with Python

To confirm correctness and explore convergence behavior, a Python script:

1. Loads the sampled data from the C++ output files.
2. Recomputes the integrals using `scipy.integrate.trapezoid` and `scipy.integrate.simpson`.
3. Implements Romberg integration manually.
4. Computes the relative errors for each method with respect to the analytical integral value.
5. Computes the absolute errors between C++ and Python results.
   
Results confirm theoretical error rates and demonstrate agreement between C++ and Python implementations.