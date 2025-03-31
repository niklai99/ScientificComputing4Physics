#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <cmath>

// Abstract base class for a mathematical function.
class Function {
public:
    virtual ~Function() = default;
    // Evaluate the function at x.
    virtual double operator()(double x) const = 0;
};

// Derived class for f(x) = cos(x) * exp(x)
class CosExpFunction : public Function {
public:
    double operator()(double x) const override {
        return std::cos(x) * std::exp(x);
    }
};

#endif  // FUNCTION_HPP
