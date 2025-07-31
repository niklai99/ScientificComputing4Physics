# Summation Algorithms Overview

Task 5a explores how different summation algorithms handle floating‑point cancellation when summing the vector

```cpp
const std::vector<double> vec = {1.0, 1.0e16, -1.0e16, -0.5};
```

whose true sum is 0.5. The code defines an abstract `Summator` interface (in `Summator.hpp`) with a single method:

```cpp
virtual double sum(const std::vector<double>& values) = 0;
```

Five concrete strategies implement this interface:

## 1. For‑Loop Summator

A straightforward loop that accumulates each element in order:

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

This approach suffers from catastrophic cancellation when a very large value overwhelms small ones.

## 2. GSL Summator

Delegates to the GNU Scientific Library’s `gsl_vector_sum` on a `gsl_vector` wrapper. It behaves like the naive loop under the hood and exhibits the same numerical issues.

```cpp
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

## 3. Pairwise Summator

Recursively splits the array in half, sums each half, then adds the two partial sums. This reduces error growth by grouping similar‑magnitude terms:

```cpp
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
```

## 4. Kahan Summator

Implements the Kahan compensated summation algorithm, maintaining a separate compensation `c` to capture lost low‑order bits:

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

## 5. Neumaier Summator

An improved Kahan variant that handles the case where the new term has larger magnitude than the running sum:

```cpp
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