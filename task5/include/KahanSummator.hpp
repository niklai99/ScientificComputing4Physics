#ifndef KAHAN_SUMMATOR_HPP
#define KAHAN_SUMMATOR_HPP

#include "Summator.hpp"
#include <vector>

// Implementation using the Kahan summation algorithm to reduce round-off error.
class KahanSummator : public Summator {
public:
    double sum(const std::vector<double>& vec) const override {
        double sum = 0.0;
        double c = 0.0; // Compensation for lost low-order bits.
        for (size_t i = 0; i < vec.size(); ++i) {
            double y = vec[i] - c;
            double t = sum + y;
            c = (t - sum) - y;
            sum = t;
        }
        return sum;
    }
};

#endif // KAHAN_SUMMATOR_HPP
