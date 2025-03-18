#ifndef VECTOR_SUM_CPP_HPP
#define VECTOR_SUM_CPP_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>

/**
 * @brief Compute the vector sum d = a * x + y.
 *
 * This function accepts a scalar and two input vectors, and writes the result into
 * the output vector d. It checks that the two input vectors have the same size;
 * if not, it throws an exception. The output vector d is resized to match the input vectors.
 *
 * @param a   Scalar multiplier.
 * @param x   Vector x.
 * @param y   Vector y.
 * @param d   Output vector to store the result.
 * @throws std::invalid_argument if the sizes of x and y are not equal.
 */
inline void vector_sum(double a, const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& d) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    d.resize(x.size());
    for (std::size_t i = 0; i < x.size(); ++i) {
        d[i] = a * x[i] + y[i];
    }
}

#endif // VECTOR_SUM_CPP_HPP