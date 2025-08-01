# Introduction to Task 6

In this task, we explore the numerical accuracy and computational behavior of two-dimensional Fast Fourier Transforms (FFTs) on real-valued data. Specifically, we:

1. Generate a real 1000×1000 matrix A whose entries are drawn from a normal distribution with mean 1 and standard deviation 1.

2. Perform a complex-to-complex (C2C) 2D FFT on A, obtaining a frequency-domain matrix C.

3. Reconstruct A by applying the inverse C2C FFT to C, then measure the root-mean-square error (RMSE) and median RMS error for both absolute and relative differences.

4. Perform a real-to-complex (R2C) trimmed FFT on A, producing R, then reconstruct A via the inverse trimmed real-to-complex-to-real (C2R) FFT.

5. Compare the C2C and R2C round-trip errors against machine precision and explain any deviations.

6. Interpret the DC component (the [0][0] coefficient) of both transforms.

7. (Bonus) On a smaller 6×6 matrix, reconstruct the full complex spectrum from the trimmed R2C output using Hermitian symmetry.


## Theoretical Background

The Discrete Fourier Transform (DFT) for an M×N array  is given by

$$
C[k,l] = \sum_{i=0}^{M-1} \sum_{j=0}^{N-1} A[i,j] e^{-2\pi i (ki/M + lj/N)}
$$

and its inverse recovers A up to a normalization factor.

The Fast Fourier Transform (FFT) computes the DFT in O(MN log(MN)) time by recursively splitting the problem into smaller DFTs (Cooley–Tukey algorithm).

For real-to-complex 2D FFTs, one exploits Hermitian symmetry to store only half the frequency-domain data (columns 0 to N/2), reducing storage and computation.

## Key Computational Steps

1. **Matrix Generation**: Create a 1000×1000 matrix A with entries from a normal distribution. Matrix generation uses `generate_gaussian_matrix(1000,1000,1.0,1.0)` from `Task06Helpers.hpp`.
2. **C2C FFT**: `FFT::fft2d` computes the full padded FFT and `FFT::ifft2d_c2c_trim` performs the inverse and crops back to the original dimensions.
3. **Error Evaluation** calls `evaluate_c2c_roundtrip(A, Arec)` and `print_error_stats` to report:
    - RMSE(abs)
    - MedianRSE(abs)
    - RMSE(rel)
    - MedianRSE(rel)
4. **R2C FFT**: `FFT::fft2d_r2c_trim` produces a trimmed spectrum R; `FFT::ifft2d_c2r_trim(R, original_cols)` reconstructs A.


## Sample Results and Interpretation

On a 1000×1000 matrix, typical results are:

```plaintext
=== c2c_trim round‑trip errors ===
  RMSE(abs) = 2.74066e-14
  MedRSE(abs)= 1.58103e-14
  RMSE(rel) = 1.54794e-11
  MedRSE(rel)= 1.39873e-14

C[0][0] = (1.00119e+06,0)  (≈ sum of A)

=== r2c round‑trip errors ===
  RMSE(abs) = 2.822e-14
  MedRSE(abs)= 1.66533e-14
  RMSE(rel) = 1.62722e-11
  MedRSE(rel)= 1.49467e-14

R[0][0] = (1.00119e+06,0)  (DC term again)
```

- The absolute RMSE of a few $10^{-14}$ indicates that we are very close to machine precision, consistent with $\mathcal{O}(\sqrt{MN\epsilon})$ accumulation of rounding errors.
- The relative RMSE of $\sim 10^{-11}$ reflects the additional $\mathcal{O}(N \log N)$ operations in the FFT.
- The DC component C[0][0] and R[0][0] both equal the sum of all entries in A.


## What you will find in this repository

- **Source Code (`src/`):**  
    Contains the C++ source files for computing the FFTs and testing the round-trip errors.

- **Header Files (`include/`):**  
    Provides declarations for the FFT methods and utilities.

- **Test Files (`test/`):**  
    Contains unit tests for the FFT implementations and error evaluations.

- **Helper Scripts (`scripts/`):**  
    - `buildProject.sh`: A script to build the project from scratch.
    - `destroyProject.sh`: A script to completely clean the project, removing build artifacts and installed commands.

- **Docker Environment (`docker/`):**  
    A Dockerfile (e.g., `Dockerfile.alma9`) is included to provide a ready-to-use development environment with all required dependencies.

- **Run Script Template (`commands/run.in`):**  
    This template is used to generate a wrapper script that is installed to `/usr/local/bin` for easy invocation of project executables.

- **`CMakeLists.txt`:**  
    The CMake build configuration file for the project.


### Project Structure

The project directory structure is as follows:

```plaintext
project/                 # Project root directory
│ 
├── commands/                # Contains the run script template
│   └── run.in                    # Script to run executables with the correct environment
├── docker/                  # Docker build context
│   └── Dockerfile.alma9          # Dockerfile for building the project
├── include/                 # Header files
│   ├── FFT.hpp                   # Declaration of the FFT class
│   ├── task06_bonus.hpp          # Declaration of the bonus task functions
│   ├── task06.hpp                # Declaration of the Task06 class
├── scripts/                 # Helper scripts
│   ├── buildProject.sh           # Script to build the project from scratch
│   └── destroyProject.sh         # Script to completely clean the project
├── src/                     # Source code files
│   ├── FFT.cpp                   # Implementation of the FFT class
│   ├── task06_bonus.cpp          # Implementation of the bonus task functions
│   ├── task06.cpp                # Main entry point for Task 6
├── test/                    # Unit tests
│   ├── test_fft_next_power_of_two.cpp        # Tests for FFT utility functions
│   ├── test_fft1d.cpp                        # Tests for 1D FFT functionality
│   ├── test_fft2d_c2c_trim.cpp               # Tests for 2D FFT functionality
│   ├── test_fft2d_c2c.cpp                    # Tests for 2D R2C FFT functionality
│   ├── test_fft2d_r2c_reconstruct_full.cpp   # Tests for 2D R2C FFT functionality
│   ├── test_fft2d_r2c_trim.cpp               # Tests for Task 6 functionality
├── CMakeLists.txt           # CMake build configuration file
└── README.md                # Project documentation  
```