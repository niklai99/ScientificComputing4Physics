#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>
#include <cmath>
#include <stdexcept>

#include "vector_sum.hpp"
#include "vector_sum_chunked.hpp"

int main() {
    // 1) Check that chunked result equals original
    {
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
    }

    // 2) n not divisible by chunk_size
    {
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
    }

    // 3) chunk_size >= n
    {
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
    }

    // 4) mismatched sizes must throw
    {
        bool caught = false;
        std::vector<double> x{1}, y{1,2}, d, partial;
        try {
            vector_sum_chunked(1.0, x, y, d, /*chunk_size=*/2, partial);
        } catch (const std::invalid_argument&) {
            caught = true;
        }
        assert(caught);
    }

    // 5) chunk_size zero must throw
    {
        bool caught = false;
        std::vector<double> x{1,2}, y{3,4}, d, partial;
        try {
            vector_sum_chunked(1.0, x, y, d, /*chunk_size=*/0, partial);
        } catch (const std::invalid_argument&) {
            caught = true;
        }
        assert(caught);
    }

    std::cout << "All chunked-vector_sum tests passed.\n";
    return 0;
}
