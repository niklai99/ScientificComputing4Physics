#ifndef VECTOR_SUM_OMP_HPP
#define VECTOR_SUM_OMP_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <omp.h>

/**
 * @brief Parallel DAXPY: d[i] = a*x[i] + y[i] using OpenMP.
 *
 * @throws std::invalid_argument if x.size() != y.size().
 */
inline void vector_sum_omp(double a,
                           const std::vector<double>& x,
                           const std::vector<double>& y,
                           std::vector<double>& d)
{
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    std::size_t n = x.size();
    d.resize(n);

    #pragma omp parallel for
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = a * x[i] + y[i];
    }
}

#endif // VECTOR_SUM_OMP_HPP
