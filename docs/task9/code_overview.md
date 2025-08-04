# Parallel DAXPY with OpenMP and MPI

In Task 9, we parallelize the fundamental DAXPY operation

$$
d_i = a \cdot x_i + y_i
$$

across multiple CPU cores using two models: OpenMP for shared‐memory threading, and MPI for distributed‐memory processes. We provide three header‐only implementations, each with a matching test driver that measures performance and verifies correctness against the serial baseline.

## Serial implementation

The baseline function lives in `vector_sum.hpp`:

```cpp title="vector_sum.hpp"
inline void vector_sum(double a, 
                       const std::vector<double>& x, 
                       const std::vector<double>& y, 
                       std::vector<double>& d) 
{
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    std::size_t n = x.size();
    d.resize(n);

    for (std::size_t i = 0; i < n; ++i) {
        d[i] = a * x[i] + y[i];
    }
}
```

## OpenMP implementation

To leverage multi‐core CPUs, we insert an OpenMP pragma:

```cpp title="vector_sum_omp.hpp"
inline void vector_sum_omp(double a,
                           const std::vector<double>& x,
                           const std::vector<double>& y,
                           std::vector<double>& d)
{
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    std::size_t n = x.size();
    d.resize(n);

    #pragma omp parallel for                // parallelize the loop with OpenMP directive
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = a * x[i] + y[i];
    }
}
```

## MPI implementation

For distributed‐memory parallelism, `vector_sum_mpi` broadcasts the input across MPI ranks and gathers the output:

```cpp title="vector_sum_mpi.hpp"
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
```

The MPI daxpy implementation relies first on the broadcast of the input vectors `x` and `y` from the root process (rank 0) to all other ranks, ensuring that each rank has access to the same data. This is perfomed by the test driver `test_vector_sum_mpi.cpp` using the MBI_Bcast function:

```cpp title="test_vector_sum_mpi.cpp"
MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Bcast(y.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
```

where `n` is the size of the vectors. After broadcasting, each rank computes its local piece of the result using the `vector_sum_mpi` function, which performs the DAXPY operation on its local copy of `x` and `y`. Finally, the results from all ranks are gathered back to the root process using `MPI_Gatherv`, which collects the local results into a single vector `d` on rank 0.