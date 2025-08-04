#ifndef VECTOR_SUM_MPI_HPP
#define VECTOR_SUM_MPI_HPP

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <mpi.h>

/**
 * @brief Distributed DAXPY: each rank computes a chunk of d = a*x + y,
 *        then root gathers the full vector.
 *
 * @throws std::invalid_argument on size mismatch (only on rank 0).
 */
inline void vector_sum_mpi(double a,
                           const std::vector<double>& x,
                           const std::vector<double>& y,
                           std::vector<double>& d)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::size_t n = x.size();
    if (x.size() != y.size()) {
        if (rank == 0) throw std::invalid_argument("Vectors x and y must have the same size.");
        else MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // split n roughly evenly
    std::size_t base = n / size; // base number of elements per rank
    std::size_t rem  = n % size; // remainder to distribute
    std::size_t start = rank * base + std::min<std::size_t>(rank, rem); // start index for this rank
    std::size_t count = base + (static_cast<std::size_t>(rank) < rem ? 1 : 0); // number of elements for this rank


    // local piece
    std::vector<double> local_d(count);
    for (std::size_t i = 0; i < count; ++i) {
        local_d[i] = a * x[start + i] + y[start + i];
    }

    // prepare for gather
    std::vector<int> counts(size), displs(size); // counts and displacements for gather
    for (int r = 0; r < size; ++r) {
        counts[r] = (n / size) + (r < (int)rem ? 1 : 0); // each rank gets base + 1 if it is in the first rem ranks
        displs[r] = r * (n / size) + std::min(r, (int)rem); // displacement for each rank
    }

    if (rank == 0) d.resize(n);
    MPI_Gatherv(local_d.data(), count, MPI_DOUBLE,
                d.data(), counts.data(), displs.data(), MPI_DOUBLE,
                0, MPI_COMM_WORLD);
}

#endif // VECTOR_SUM_MPI_HPP
