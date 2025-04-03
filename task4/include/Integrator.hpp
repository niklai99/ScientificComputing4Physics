#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "Function.hpp"
#include <stdexcept>

// Class for numerical integration.
class Integrator {
public:
    // Construct with a reference to a function to integrate.
    explicit Integrator(const Function& func) : function(func) {}

    // Trapezoidal rule integration.
    double integrateTrapz(double a, double b, int n) const {
        if(n < 2) {
            throw std::invalid_argument("Number of sampling points must be at least 2.");
        }
        double h = (b - a) / (n - 1);
        double sum = 0.5 * (function(a) + function(b));
        for (int i = 1; i < n - 1; ++i) {
            double xi = a + i * h;
            sum += function(xi);
        }
        return sum * h;
    }

    // Simpson's rule integration.
    double integrateSimpson(double a, double b, int n) const {
        // Ensure at least 3 points.
        if(n < 3) {
            throw std::invalid_argument("Number of sampling points must be at least 3.");
        }
        // Simpson's rule requires an odd number of points.
        // If n is even, simply increment n by 1.
        if ((n - 1) % 2 != 0) {
            ++n;
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

    // Romberg integration.
    // Uses the trapezoidal rule with n = 2^i + 1 sampling points.
    // maxIter sets the maximum number of Romberg iterations.
    // tol specifies the convergence tolerance.
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

private:
    const Function& function;
};

#endif  // INTEGRATOR_HPP
