# Pairwise Summation

This page describes the Pairwise summation strategy, which reduces rounding error by recursively combining sub-sums of similar size.

## Implementation

Pairwise summation works by dividing the input array into two halves, summing each half separately, and then adding the two partial sums. By pairing numbers of comparable magnitude, the method limits the error growth that occurs when small values are added to a large accumulator.

```cpp linenums="1" title="PairwiseSummator.hpp"
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

## Numerical Behavior

When applied to the test vector

```cpp
const std::vector<double> vec = {1.0, 1.0e16, -1.0e16, -0.5};
``` 

the algorithm

1. splits the vector into two halves: `{1.0, 1.0e16}` and `{-1.0e16, -0.5}`
2. sums left half to get `1.0 + 1.0e16 = 1.0e16`
3. sums right half to get `-1.0e16 - 0.5 = -1.0e16`
4. finally adds the two partial sums: `1.0e16 + (-1.0e16) = 0.0`

In this extreme case, the pairwise summation still results in catastrophic cancellation. However, for larger and more varied datasets, this method significantly reduces overall rounding error compared to the naive loop.