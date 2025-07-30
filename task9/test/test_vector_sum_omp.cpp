#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>

#include "vector_sum.hpp"
#include "vector_sum_omp.hpp"

int main() {
    std::vector<std::size_t> sizes = {1000000, 2000000, 5000000};
    double a = 2.5;

    for (auto n : sizes) {
        std::vector<double> x(n), y(n), d_ser, d_omp;
        for (std::size_t i = 0; i < n; ++i) {
            x[i] = i * 0.001;
            y[i] = n - i * 0.001;
        }

        auto t0 = std::chrono::high_resolution_clock::now();
        vector_sum(a, x, y, d_ser);
        auto t1 = std::chrono::high_resolution_clock::now();
        vector_sum_omp(a, x, y, d_omp);
        auto t2 = std::chrono::high_resolution_clock::now();

        assert(d_ser == d_omp);

        auto dt_ser = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        auto dt_omp = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        std::cout << "n=" << n 
                  << " serial=" << dt_ser << "ms"
                  << "  omp="    << dt_omp << "ms\n";
    }
    return 0;
}
