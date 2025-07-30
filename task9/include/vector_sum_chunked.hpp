#ifndef VECTOR_SUM_CHUNKED_HPP
#define VECTOR_SUM_CHUNKED_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <algorithm>

/**
 * @brief Compute d = a*x + y in fixed‑size chunks and record partial sums.
 *
 * Splits the work over chunks of length chunk_size.  For each chunk it
 * computes the DAXPY update on d and sums the updated elements into
 * partial_chunk_sum[chunk_index].  The caller can then verify that
 * sum(partial_chunk_sum) equals sum(d).
 *
 * @param a   Scalar multiplier.
 * @param x   Input vector x.
 * @param y   Input vector y.
 * @param d   Output vector (resized to x.size()).
 * @param chunk_size  Number of elements per chunk (must be ≥ 1).
 * @param partial_chunk_sum
 *            Output vector of length ceil(x.size()/chunk_size),
 *            where each entry is the sum of d over one chunk.
 * @throws std::invalid_argument if x.size() ≠ y.size() or chunk_size == 0.
 */
inline void vector_sum_chunked(double a,
                               const std::vector<double>& x,
                               const std::vector<double>& y,
                               std::vector<double>& d,
                               std::size_t chunk_size,
                               std::vector<double>& partial_chunk_sum)
{
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    if (chunk_size == 0) {
        throw std::invalid_argument("chunk_size must be at least 1.");
    }

    std::size_t n = x.size();
    d.resize(n);

    // Number of chunks = ceil(n / chunk_size)
    std::size_t num_chunks = (n + chunk_size - 1) / chunk_size;
    partial_chunk_sum.assign(num_chunks, 0.0);

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
}

#endif // VECTOR_SUM_CHUNKED_HPP
