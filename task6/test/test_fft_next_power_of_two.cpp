// test/test_fft_next_power_of_two.cpp

#include <cassert>
#include <iostream>
#include "FFT.hpp"

int main() {
    assert(FFT::next_power_of_two(0)  == 1);
    assert(FFT::next_power_of_two(1)  == 1);
    assert(FFT::next_power_of_two(2)  == 2);
    assert(FFT::next_power_of_two(3)  == 4);
    assert(FFT::next_power_of_two(5)  == 8);
    assert(FFT::next_power_of_two(16) == 16);
    assert(FFT::next_power_of_two(17) == 32);
    std::cout << "✔ next_power_of_two() tests passed\n";
    return 0;
}
