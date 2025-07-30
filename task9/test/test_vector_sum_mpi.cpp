#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <mpi.h>

#include "vector_sum.hpp"
#include "vector_sum_mpi.hpp"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::size_t> sizes = {1000000, 2000000, 5000000};
    double a = 1.1;

    for (auto n : sizes) {
        std::vector<double> x, y, d_ser, d_mpi;
        if (rank == 0) {
            x.resize(n);
            y.resize(n);
            for (std::size_t i = 0; i < n; ++i) {
                x[i] = i * 0.002;
                y[i] = n - i * 0.002;
            }
        }
        // broadcast inputs
        x.resize(n);
        y.resize(n);
        MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(y.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            auto t0 = std::chrono::high_resolution_clock::now();
            vector_sum(a, x, y, d_ser);
            auto t1 = std::chrono::high_resolution_clock::now();
            vector_sum_mpi(a, x, y, d_mpi);
            auto t2 = std::chrono::high_resolution_clock::now();

            assert(d_ser == d_mpi);

            auto dt_ser = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            auto dt_mpi = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

            std::cout << "[rank 0] n=" << n
                      << " serial=" << dt_ser << "ms"
                      << "  mpi="    << dt_mpi << "ms\n";
        } else {
            // non‐root just calls the MPI version
            vector_sum_mpi(a, x, y, d_mpi);
        }
    }

    MPI_Finalize();
    return 0;
}
