# Introduction to Task 5

Task 5 examines two problems in numerical computing with floating‑point arithmetic:

1. **Summation Accuracy (Task 5a):** We evaluate how different summation algorithms handle catastrophic cancellation and rounding error when summing vectors of mixed magnitude.

2. **Randomized DAXPY Validation (Task 5b):** We generate Gaussian random vectors and apply the DAXPY operation. To validate the results, we compute the sample mean and standard deviation, comparing them to theoretical expectations.

## Task 5a: Summation Methods

Floating‑point addition is not associative; summing a large value followed by a much smaller one can lose the contribution of the small term. To explore this effect, we sum the test vector:

```
{1.0, 1e16, -1e16, -0.5}
```

whose mathematically exact sum is `0.5`, but naive accumulation yields `-0.5`. We implement and compare five strategies, all conforming to the `Summator` interface:

- **Naive Summation:** Directly sums the vector elements.
- **GSL Summation:** Uses the GNU Scientific Library's `gsl_sum` function
- **Pairwise Summation:** Recursively sums pairs of elements to reduce error.
- **Kahan Summation:** Maintains a running compensation for lost low-order bits.
- **Neumaier Summation:** Similar to Kahan but uses a different compensation strategy.

The C++ driver `computeSum.cpp` instantiates each `Summator`, applies it to the test vector, and prints the results. Subsequent sections document each algorithm’s design and demonstrate how only Neumaier’s method recovers the correct value.

## Task 5b: DAXPY with Random Vectors

In Task 5b, we:

1. Generate two vectors of length `n` with independent samples from a Gaussian distribution with mean `0` and standard deviation `1`, using the `<random>` library as shown in `VectorGenerator.hpp`.
2. Apply the DAXPY operation, which computes `y = a * x + y`, where `a` is a scalar, and `x` and `y` are vectors.
3. Test each strategy via the harness in `TestSuite.hpp`, invoked by `testDaxpy.cpp`, over multiple iterations. For each dimension `d`, we compute the sample mean and standard deviation.
4. Validate against the theoretical expectation of $d \sim \mathcal{N}(0, \sqrt{a^2 + 1})$.

## What you will find in this repository

- **Source Code (`src/`):**  
    Contains the C++ source files for computing the sum using the five methods and for testing the DAXPY operation with random vectors.

- **Header Files (`include/`):**  
    Provides declarations for the summation methods, DAXPY operation, and vector generation.

- **Configuration Files (`config/`):**  
    A YAML file (e.g., `config.yml`) that specifies the integration parameters, including the function to integrate, the interval, the number of points (N), and output options.

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
│   └── run.in                   # Script to run executables with the correct environment
├── config/                  # Configuration files
│   └── config.yml/              # Example configuration file
├── docker/                  # Docker build context
│   └── Dockerfile.alma9         # Dockerfile for building the project
├── include/                 # Header files
│   ├── ForLoopSummator.hpp      # Declaration of the ForLoopSummator class
│   ├── GSLSummator.hpp          # Declaration of the GSLSummator class
│   ├── KahanSummator.hpp        # Declaration of the KahanSummator class
│   ├── NeumaierSummator.hpp     # Declaration of the NeumaierSummator class
│   ├── PairwiseSummator.hpp     # Declaration of the PairwiseSummator class
│   ├── Summator.hpp             # Declaration of the Summator interface
│   ├── TestSuite.hpp            # Declaration of the TestSuite class
│   ├── VectorGenerator.hpp      # Declaration of the VectorGenerator class
│   ├── VectorSumDefault.hpp     # Declaration of the VectorSumDefault class
│   ├── VectorSumGSL.hpp         # Declaration of the VectorSumGSL class
│   ├── VectorSumInterface.hpp   # Declaration of the VectorSum interface
├── scripts/                 # Helper scripts
│   ├── buildProject.sh          # Script to build the project from scratch
│   └── destroyProject.sh        # Script to completely clean the project
├── src/                     # Source code files
│   ├── computeSum.cpp           # Code to compute the sum using the five methods
│   ├── testDaxpy.cpp            # Code to test the DAXPY operation with random vectors
├── CMakeLists.txt           # CMake build configuration file
└── README.md                # Project documentation
```