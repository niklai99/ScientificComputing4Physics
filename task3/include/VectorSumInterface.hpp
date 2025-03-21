#ifndef VECTOR_SUM_INTERFACE_HPP
#define VECTOR_SUM_INTERFACE_HPP

#include <vector>
#include <cstddef>

/**
 * @brief Abstract interface for computing the vector sum.
 *
 * Defines a common interface with a virtual method compute_sum().
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
