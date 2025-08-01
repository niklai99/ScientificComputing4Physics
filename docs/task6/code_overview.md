# Code Overview for Task 6

In Task 6, we implement and evaluate two-dimensional Fast Fourier Transforms (FFTs) on real-valued matrices, using both complex-to-complex (C2C) and real-to-complex (R2C) approaches. The core functionality resides in `FFT.cpp` and its header `FFT.hpp`, while the drivers `task06.cpp` and `task06_bonus.cpp` invoke these routines and report accuracy metrics.

## Main steps

### Data generation

We begin by creating a 1000×1000 matrix A with entries drawn from $\mathcal{N}(1,1)$. In `Task06Helpers.hpp`, the function:

```cpp linenums="1"
inline std::vector<std::vector<double>>
generate_gaussian_matrix(size_t M,
                         size_t N,
                         double mean,
                         double stddev)
{
    std::mt19937_64 gen{std::random_device{}()};
    std::normal_distribution<double> dist{mean, stddev};
    std::vector<std::vector<double>> A(M, std::vector<double>(N));
    for (auto& row : A)
        for (auto& x : row)
            x = dist(gen);
    return A;
}
```

produces A, which is then passed to the FFT routines.

### Complex-to-complex FFT roundtrip

In `FFT.cpp`, we implement `fft2d_c2c_trim` and `ifft2d_c2c_trim`. The forward transform:

```cpp
std::vector<std::vector<std::complex<double>>> C_trim = FFT::fft2d_c2c_trim(A);
```

computes the full 2D FFT by applying 1D FFTs first on each row and then on each column. To revert to the spatial domain we call:

```cpp
std::vector<std::vector<std::complex<double>>> Arec_c2c = FFT::ifft2d_c2c_trim(C_trim);
```

which applies the inverse FFT.

### Real-to-complex FFT roundtrip

To exploit symmetry and reduce storage, we use the R2C variant. The forward call:

```cpp
std::vector<std::vector<std::complex<double>>> R_half = FFT::fft2d_r2c_trim(A);
```

computes a half-spectrum of size M×(N/2+1). We then reconstruct A with:

```cpp
std::vector<std::vector<std::complex<double>>> Arec_r2c = FFT::ifft2d_r2c_trim(R_half, COLS);
``` 

which performs the inverse real-to-complex transform and restores the full matrix.


### Error evaluation

In `Task06Helpers.hpp`, the function `evaluate_c2c_roundtrip` takes A and Arec and computes statistics:

```cpp
auto stats_c2c = evaluate_c2c_roundtrip(A, Arec_c2c);
print_error_stats("c2c_trim round‑trip errors", stats_c2c);
```

These routines calculate the root-mean-square (RMSE), median RMS error, and relative counterparts, printing results to the console.

Similarly, for the R2C roundtrip:

```cpp
auto stats_r2c = evaluate_r2c_roundtrip(A, Arec_r2c);
print_error_stats("r2c round‑trip errors", stats_r2c);
```

### Bonus: Hermitian symmetry

When the input matrix A is real, the FFT C exhibits Hermitian symmetry. The trimmed spectrum R can be expanded to the full complex matrix C via Hermitian symmetry. The function:

```cpp
std::vector<std::vector<std::complex<double>>> C_from_R = FFT::r2c_reconstruct_full(R_half);
```

reconstructs the full complex matrix C from the half-spectrum R_half, leveraging the symmetry property.

