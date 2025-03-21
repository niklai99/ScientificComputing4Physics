#ifndef VECTOR_SUM_DEFAULT_HPP
#define VECTOR_SUM_DEFAULT_HPP

#include "VectorSumInterface.hpp"

/**
 * @brief Default vector sum implementation: d = a * x + y.
 *
 * This class implements the VectorSumInterface using a simple element‐by‐element loop.
 * Each element in the output vector d is computed as:
 *
 *     d[i] = a * x[i] + y[i]
 *
 */
class VectorSumDefault : public VectorSumInterface {
public:
    virtual void compute_sum(const std::vector<double>& x,
                             const std::vector<double>& y,
                             double a,
                             std::vector<double>& d) override {
        // Determine the number of elements.
        size_t N = x.size();
        // Resize the output vector to match.
        d.resize(N);
        // Compute each element of d.
        for (size_t i = 0; i < N; ++i) {
            d[i] = a * x[i] + y[i];
        }
    }
};

#endif // VECTOR_SUM_DEFAULT_HPP
