#ifndef SUMMATOR_HPP
#define SUMMATOR_HPP

#include <vector>

// Abstract base class for a summation algorithm.
class Summator {
public:
    virtual ~Summator() = default;
    // Return the sum of the elements in vec.
    virtual double sum(const std::vector<double>& vec) const = 0;
};

#endif // SUMMATOR_HPP
