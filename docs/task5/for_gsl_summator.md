# Naive and GSL Summation Methods

This page documents the two simplest summation strategies: the naive for‑loop approach and the GSL-based summator. 

## For-loop Summator

The straightforward approach accumulates values in the order given. While easy to implement, it is vulnerable to catastrophic cancellation when summing very large and very small magnitudes in sequence.

```cpp
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
```

When summing the vector

```cpp
const std::vector<double> vec = {1.0, 1.0e16, -1.0e16, -0.5};
```

the accumulator becomes `1.0`. Then, adding `1.0e16` results in `1.0e16`, which is then followed by subtracting `1.0e16`, leaving the accumulator at `0.0`. Finally, subtracting `0.5` yields `-0.5`, which is far from the true sum of `0.5`. This shows cancellation of small values against large ones.

## GSL Summator

To leverage existing library routines, this class wraps the GNU Scientific Library (GSL) summation function. However, `gsl_vector_sum` internally performs a naive loop, so numerical behavior mirrors the basic approach.

```cpp
#include <gsl/gsl_vector.h>

class GSLSummator : public Summator {
public:
    double sum(const std::vector<double>& vec) const override {
        size_t N = vec.size();
        gsl_vector* v = gsl_vector_alloc(N);
        for (size_t i = 0; i < N; ++i) {
            gsl_vector_set(v, i, vec[i]);
        }
        double s = gsl_vector_sum(v);  // Uses GSL's vector-sum routine.
        gsl_vector_free(v);
        return s;
    }
};
```

Running on the test vector yields the same intermediate lost precision. When `gsl_vector_sum` loops: `acc = 1.0`, then `1e16`, then `0.0`, then `-0.5`.

