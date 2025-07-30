#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>
#include "vector_sum.hpp"

int main() {
    // Test 1: basic functionality
    {
        double a = 2.0;
        std::vector<double> x{1.0, 2.0, 3.0};
        std::vector<double> y{4.0, 5.0, 6.0};
        std::vector<double> d;
        vector_sum(a, x, y, d);
        assert(d.size() == x.size());
        assert(d[0] == 6.0);
        assert(d[1] == 9.0);
        assert(d[2] == 12.0);
    }

    // Test 2: empty vectors
    {
        double a = 5.0;
        std::vector<double> x, y, d;
        vector_sum(a, x, y, d);
        assert(d.empty());
    }

    // Test 3: mismatched sizes must throw
    {
        bool thrown = false;
        std::vector<double> x{1.0};
        std::vector<double> y{1.0, 2.0};
        std::vector<double> d;
        try {
            vector_sum(1.0, x, y, d);
        } catch (const std::invalid_argument&) {
            thrown = true;
        }
        assert(thrown);
    }

    std::cout << "All vector_sum tests passed.\n";
    return 0;
}
