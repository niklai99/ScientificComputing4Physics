# Kahan Summation

This page explains the Kahan summation algorithm, which greatly reduces rounding error by keeping a running compensation for lost low-order bits.

## Implementation

When adding a small value to a large accumulator, the small value’s low-order bits can be lost due to finite precision. Kahan’s method maintains an extra variable `c` that tracks this lost error and reintroduces it in subsequent additions.

```cpp
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
```

## Numerical Behavior

When applied to the test vector

```cpp
const std::vector<double> vec = {1.0, 1.0e16, -1.0e16, -0.5};
``` 

the computed sum remains `-0.5`, the same as the naive approach. 

| Step             | Value Added (`v`) | `sum` after addition | Compensation (`c`) |
|------------------|-------------------|----------------------|--------------------|
| Initial          | —                 | 0.0                  | 0.0                |
| Add `1.0`        | 1.0               | 1.0                  | 0.0                |
| Add `1e16`       | 1e16              | 1e16                 | 0.0                |
| Add `-1e16`      | -1e16             | 0.0                  | 0.0                |
| Add `-0.5`       | -0.5              | -0.5                 | 0.0                |
