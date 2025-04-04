#ifndef FORLOOP_SUMMATOR_HPP
#define FORLOOP_SUMMATOR_HPP

#include "Summator.hpp"

// Implementation that sums the vector using a plain for loop.
class ForLoopSummator : public Summator {
public:
    double sum(const std::vector<double>& vec) const override {
        double s = 0.0;
        for (size_t i = 0; i < vec.size(); ++i) {
            s += vec[i];
        }
        return s;
    }
};

#endif // FORLOOP_SUMMATOR_HPP
