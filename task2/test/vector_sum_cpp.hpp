#ifndef VECTOR_SUM_CPP_HPP
#define VECTOR_SUM_CPP_HPP

#include <vector>
#include <cstddef>

/**
 * @brief Compute the vector sum d = a * x + y.
 *
 * This function creates two vectors of length n where every element of x is set to x_val and every element
 * of y is set to y_val. It then computes the result as:
 *     d[i] = a * x_val + y_val, for i = 0 to n-1.
 *
 * @param a      Scalar multiplier.
 * @param x_val  Constant value to fill vector x.
 * @param y_val  Constant value to fill vector y.
 * @param n      Dimension of the vectors.
 * @return std::vector<double> The resulting vector d.
 */
inline std::vector<double> vector_sum(double a, double x_val, double y_val, std::size_t n) {
    std::vector<double> d(n);
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = a * x_val + y_val;
    }
    return d;
}

#endif // VECTOR_SUM_CPP_HPP
