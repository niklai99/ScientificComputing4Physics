# Trapezoidal and Simpson integration 

This page documents how the `Integrator` class implements the Trapezoidal and Simpson’s rules for numerical integration. All code resides in `include/Integrator.hpp`.

## Trapezoidal Rule

The trapezoidal rule approximates the integral

$$
\int_a^b f(x) \, dx 
$$

by summing the areas of trapezoids under the curve. In `Integrator::integrateTrapz()`:

```cpp
// Trapezoidal rule integration.
double integrateTrapz(double a, double b, int n) const {
    if (n < 2) {
        throw std::invalid_argument(
            "Number of sampling points must be at least 2.");
    }
    double h = (b - a) / (n - 1);
    double sum = 0.5 * (function(a) + function(b));
    for (int i = 1; i < n - 1; ++i) {
        double xi = a + i * h;
        sum += function(xi);
    }
    return sum * h;
}
```

## Simpson's Rule

Simpson’s rule provides a more accurate approximation by using parabolic segments. It requires an odd number of sampling points. In `Integrator::integrateSimpson()`:

```cpp
// Simpson's rule integration.
double integrateSimpson(double a, double b, int n) const {
    // Ensure at least 3 points (n odd).
    if (n < 3) {
        throw std::invalid_argument(
            "Number of sampling points must be at least 3.");
    }
    if (n % 2 == 0) {
        ++n;  // Make n odd by adding one point
    }
    double h = (b - a) / (n - 1);
    double sum = function(a) + function(b);
    for (int i = 1; i < n - 1; ++i) {
        double xi = a + i * h;
        if (i % 2 == 0) {
            sum += 2 * function(xi);
        } else {
            sum += 4 * function(xi);
        }
    }
    return (h / 3.0) * sum;
}
```