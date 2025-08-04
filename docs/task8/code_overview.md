# Chunked Vector Summation 

In Task 8, we extend the basic DAXPY operation to process the input vectors in fixed‐size chunks. The key function is declared in `vector_sum_chunked.hpp` as:

```cpp title="vector_sum_chunked.hpp"
inline void vector_sum_chunked(double a,
                               const std::vector<double>& x,
                               const std::vector<double>& y,
                               std::vector<double>& d,
                               std::size_t chunk_size,
                               std::vector<double>& partial_chunk_sum)
```

This function splits the work of computing 

$$
d_i = a \cdot x_i + y_i
$$

into consecutive blocks of size `chunk_size`. Processing in chunks brings benefits in data locality and paves the way for parallel execution, at the cost of extra control logic and a secondary reduction step.

## Parameter validation and setup

The first steps ensure the inputs are well‐formed and prepare the outputs:

```cpp title="vector_sum_chunked.hpp"
if (x.size() != y.size()) {
    throw std::invalid_argument("Vectors x and y must have the same size.");
}
if (chunk_size == 0) {
    throw std::invalid_argument("chunk_size must be at least 1.");
}

auto n = x.size();
d.resize(n);

// Compute how many chunks are required (ceiling division)
std::size_t num_chunks = (n + chunk_size - 1) / chunk_size;
// Initialize partial sums to zero
partial_chunk_sum.assign(num_chunks, 0.0);
```

- We throw `std::invalid_argument` on mismatched input sizes or zero chunk size.
- The output vector `d` is resized to match `x.size()`.
- `num_chunks` is computed as `(n + chunk_size - 1) / chunk_size`, and `partial_chunk_sum` is initialized accordingly.


## Chunk processing loop

The core loop iterates over each chunk, computes the subarray update, and records the chunk’s sum:

```cpp title="vector_sum_chunked.hpp"
for (std::size_t chunk = 0; chunk < num_chunks; ++chunk) {
    std::size_t start = chunk * chunk_size;
    std::size_t end   = std::min(start + chunk_size, n);
    double sum = 0.0;
    for (std::size_t i = start; i < end; ++i) {
        d[i] = a * x[i] + y[i];
        sum += d[i];
    }
    partial_chunk_sum[chunk] = sum;
}
```

Within each chunk:

- We compute the starting index `start` and the ending index `end`, ensuring the last chunk may be shorter.
- We apply the DAXPY update to each element in `[start, end)` and accumulate these into `sum`.
- We store the partial sum for this chunk in `partial_chunk_sum`.

## Testing the chunked vector sum

Unit tests in `test_vector_sum.cpp` validate the `vector_sum_chunked` function for various scenarios, ensuring correctness and robustness, often comparing it against a standard DAXPY implementation.

### Basic comparison

Verifies that for a simple input, the chunked version matches the standard loop:

```cpp title="test_vector_sum.cpp"
double a = 1.5;
std::vector<double> x{0,1,2,3,4,5};
std::vector<double> y{6,7,8,9,10,11};
std::vector<double> d_ref, d_chk, partial;

vector_sum(a, x, y, d_ref);
vector_sum_chunked(a, x, y, d_chk, /*chunk_size=*/3, partial);

assert(d_ref == d_chk);

double sum_ref = std::accumulate(d_ref.begin(), d_ref.end(), 0.0);
double sum_par = std::accumulate(partial.begin(), partial.end(), 0.0);
assert(std::fabs(sum_ref - sum_par) < 1e-12);
```

This test confirms both the element-wise results and the aggregated partial sums are consistent.

### Non-divisible chunk sizes

Ensures correct inexact chunking when `chunk_size` does not evenly divide the vector length:

```cpp title="test_vector_sum.cpp"
double a = 2.0;
std::vector<double> x{1,2,3,4,5};
std::vector<double> y{5,4,3,2,1};
std::vector<double> d_ref, d_chk, partial;

vector_sum(a, x, y, d_ref);
vector_sum_chunked(a, x, y, d_chk, /*chunk_size=*/2, partial);

assert(d_ref == d_chk);
double sum_ref = std::accumulate(d_ref.begin(), d_ref.end(), 0.0);
double sum_par = std::accumulate(partial.begin(), partial.end(), 0.0);
assert(std::fabs(sum_ref - sum_par) < 1e-12);
assert(partial.size() == 3);  // chunks of sizes 2,2,1
```

### Chunk size larger than vector

Tests the edge case where `chunk_size` exceeds the vector length, ensuring it processes as a single chunk:

```cpp title="test_vector_sum.cpp"
double a = -1.0;
std::vector<double> x{1,2,3};
std::vector<double> y{3,2,1};
std::vector<double> d_ref, d_chk, partial;

vector_sum(a, x, y, d_ref);
vector_sum_chunked(a, x, y, d_chk, /*chunk_size=*/10, partial);

assert(d_ref == d_chk);
double sum_ref = std::accumulate(d_ref.begin(), d_ref.end(), 0.0);
double sum_par = std::accumulate(partial.begin(), partial.end(), 0.0);
assert(std::fabs(sum_ref - sum_par) < 1e-12);
assert(partial.size() == 1);
```

### Mismatched vector sizes

Tests that the function correctly throws an exception when input vectors differ in size:

```cpp title="test_vector_sum.cpp"
bool caught = false;
std::vector<double> x{1}, y{1,2}, d, partial;
try {
    vector_sum_chunked(1.0, x, y, d, /*chunk_size=*/2, partial);
} catch (const std::invalid_argument&) {
    caught = true;
}
assert(caught);
```

### Zero chunk size

Tests that a zero `chunk_size` throws an exception:

```cpp title="test_vector_sum.cpp"
bool caught = false;
std::vector<double> x{1,2}, y{3,4}, d, partial;
try {
    vector_sum_chunked(1.0, x, y, d, /*chunk_size=*/0, partial);
} catch (const std::invalid_argument&) {
    caught = true;
}
assert(caught);
```