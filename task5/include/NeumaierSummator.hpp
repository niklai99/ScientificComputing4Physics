#ifndef NEUMAIER_SUMMATOR_HPP
#define NEUMAIER_SUMMATOR_HPP

#include "Summator.hpp"
#include <vector>
#include <cmath>

// NeumaierSummator implements the improved Kahan–Babuška (Neumaier) summation algorithm.
class NeumaierSummator : public Summator {
public:
    double sum(const std::vector<double>& vec) const override {
        double sum = 0.0;
        double c = 0.0;  // Running compensation for lost low-order bits.
        
        for (size_t i = 0; i < vec.size(); ++i) {
            double t = sum + vec[i];
            if (std::fabs(sum) >= std::fabs(vec[i])) {
                c += (sum - t) + vec[i];
            } else {
                c += (vec[i] - t) + sum;
            }
            sum = t;
        }
        return sum + c;  // Apply the correction once at the end.
    }
};

#endif // NEUMAIER_SUMMATOR_HPP
