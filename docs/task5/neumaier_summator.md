# Neumaier Summation

This page describes the Neumaier summation algorithm, an extension of Kahan’s method that handles cases where the next term has greater magnitude than the running sum.

## Implementation

While Kahan Summation compensates for lost low-order bits, it can still mis-handle cases where a very large term follows a smaller running total. Neumaier’s algorithm adds logic to adjust the compensation when the new term is larger in magnitude than the current sum.

```cpp linenums="1" title="NeumaierSummator.hpp"
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
```

## Numerical Behavior


For the vector:

```cpp
const std::vector<double> vec = {1.0, 1.0e16, -1.0e16, -0.5};
``` 

Neumaier summation proceeds as follows:

1. **Initial**: `sum = 0.0`, `c = 0.0`
2. **Add `1.0`**:
      - `t = 0.0 + 1.0 = 1.0`
      - `c = 0.0 + (0.0 - 1.0) + 1.0 = 0.0`
      - `sum = 1.0`
3. **Add `1e16`**:
      - `t = 1.0 + 1e16 = 1e16`
      - since `|sum| < |v|`, update `c += (1e16 - 1e16) + 1.0 = 1.0`
      - `sum = 1e16`
4. **Add `-1e16`**:
      - `t = 1e16 - 1e16 = 0.0`
      - since `|sum| >= |v|`, update `c += (sum - t) + v = (1e16 - 0.0) + (-1e16) = 0.0`
      - `sum = 0.0`
5. **Add `-0.5`**:
      - `t = 0.0 - 0.5 = -0.5`
      - since `|sum| >= |v|`, update `c += (sum - t) + v = (0.0 - (-0.5)) + (-0.5) = 0.0`
      - `sum = -0.5`
 - **Final Result**: `sum + c = -0.5 + 1.0 = 0.5`

Thus, Neumaier’s method recovers the correct sum `0.5`, by preserving the lost `1.0` in the compensation variable `c` through the first addition.