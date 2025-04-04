#ifndef PAIRWISE_SUMMATOR_HPP
#define PAIRWISE_SUMMATOR_HPP

#include "Summator.hpp"
#include <vector>
#include <cstddef>

// PairwiseSummator implements the pairwise summation algorithm.
// This algorithm recursively divides the vector into two halves, 
// sums each half, and combines the results. It tends to be more 
// numerically stable than a simple iterative summation.
class PairwiseSummator : public Summator {
public:
    double sum(const std::vector<double>& vec) const override {
        return pairwiseSum(vec, 0, vec.size());
    }

private:
    // Recursively sums the elements in vec from index [start, end).
    double pairwiseSum(const std::vector<double>& vec, std::size_t start, std::size_t end) const {
        std::size_t n = end - start;
        if (n == 0)
            return 0.0;
        if (n == 1)
            return vec[start];
        // Split the array into two halves.
        std::size_t mid = start + n / 2;
        double leftSum = pairwiseSum(vec, start, mid);
        double rightSum = pairwiseSum(vec, mid, end);
        return leftSum + rightSum;
    }
};

#endif // PAIRWISE_SUMMATOR_HPP
