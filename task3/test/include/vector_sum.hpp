#ifndef VECTOR_SUM_HPP
#define VECTOR_SUM_HPP

#include <vector>
#include <cstddef>

/**
 * @brief Compute the vector sum d = a * x + y.
 *
 * This default implementation computes d element-wise.
 *
 * @param x Input vector x.
 * @param y Input vector y.
 * @param a Scalar multiplier.
 * @param d Output vector d (resized to match x and y).
 */
inline void default_vector_sum(const std::vector<double>& x,
                               const std::vector<double>& y,
                               double a,
                               std::vector<double>& d) {
    size_t N = x.size();
    d.resize(N);
    for (size_t i = 0; i < N; ++i) {
        d[i] = a * x[i] + y[i];
    }
}

#endif // VECTOR_SUM_HPP