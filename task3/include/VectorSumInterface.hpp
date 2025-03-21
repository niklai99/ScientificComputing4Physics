#ifndef VECTOR_SUM_INTERFACE_HPP
#define VECTOR_SUM_INTERFACE_HPP

#include <vector>
#include <cstddef>

/**
 * @brief Abstract interface for computing the vector sum.
 *
 * This interface defines a common method compute_sum() which all vector sum implementations
 * must override. It enforces that any derived class accepts two input vectors (x and y), a scalar (a),
 * and produces an output vector (d) according to the formula:
 *
 *     d = a * x + y
 *
 * It allows the rest of the application to work with a single, unified interface regardless
 *   of the underlying implementation (e.g., default or GSL-based).
 * It provides flexibility to easily extend or switch implementations at runtime.
 */
class VectorSumInterface {
public:
    virtual ~VectorSumInterface() {}
    virtual void compute_sum(const std::vector<double>& x,
                             const std::vector<double>& y,
                             double a,
                             std::vector<double>& d) = 0;
};

#endif // VECTOR_SUM_INTERFACE_HPP
