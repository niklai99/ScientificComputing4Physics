#ifndef VECTOR_SUM_DEFAULT_HPP
#define VECTOR_SUM_DEFAULT_HPP

#include "VectorSumInterface.hpp"

/**
 * @brief Default vector sum implementation: d = a * x + y.
 */
class VectorSumDefault : public VectorSumInterface {
public:
    virtual void compute_sum(const std::vector<double>& x,
                             const std::vector<double>& y,
                             double a,
                             std::vector<double>& d) override {
        size_t N = x.size();
        d.resize(N);
        for (size_t i = 0; i < N; ++i) {
            d[i] = a * x[i] + y[i];
        }
    }
};

#endif // VECTOR_SUM_DEFAULT_HPP
