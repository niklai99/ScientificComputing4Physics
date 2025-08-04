# Introduction to Task 8

In Task 8, we enhance the basic DAXPY-style vector summation by splitting the work into fixed‐size chunks. Instead of one loop over all n elements:


```pseudo
for i in 0..n-1:
    d[i] = a*x[i] + y[i]
```


we process blocks of size `chunk_size`, computing each block separately, and then summing the per-block results. This approach offers:

1. **Improved locality:** Operating on a small contiguous subarray at a time can boost cache‐reuse.
2. **Parallelism readiness:** Independent chunks set the stage for simple multi‐threading (each chunk can be processed concurrently).
3. **Overhead:** Calculating chunk boundaries and managing a secondary “partial sums” array adds control‐flow complexity.


## Chunked summation logic

The function signature in `vector_sum_chunked.hpp` is:

```cpp title="vector_sum_chunked.hpp"
void vector_sum_chunked(double a,
                        const std::vector<double>& x,
                        const std::vector<double>& y,
                        std::vector<double>& d,
                        std::size_t chunk_size,
                        std::vector<double>& partial_chunk_sum)
```

### Key steps

1. **Validate inputs:** 

    ```cpp title="vector_sum_chunked.hpp"
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    if (chunk_size == 0) {
        throw std::invalid_argument("chunk_size must be at least 1.");
    }
    ```

2. **Determine number of chunks:**

    ```cpp title="vector_sum_chunked.hpp"
    std::size_t n = x.size();
    d.resize(n);

    // Number of chunks = ceil(n / chunk_size)
    std::size_t num_chunks = (n + chunk_size - 1) / chunk_size;
    partial_chunk_sum.assign(num_chunks, 0.0);
    ```

3. **Process each chunk:**

    ```cpp title="vector_sum_chunked.hpp"
    for (std::size_t chunk = 0; chunk < num_chunks; ++chunk) {
        std::size_t start = chunk * chunk_size;
        std::size_t end   = std::min(start + chunk_size, n);
        double sum = 0.0;

        for (std::size_t i = start; i < end; ++i) {
            d[i] = a * x[i] + y[i];
            sum += d[i];
        }
        partial_chunk_sum[chunk] = sum;
    }
    ```

## Findings 

In tests (see `test_vector_sum.cpp`), the chunked and original implementations produce identical `d` vectors, and the sum of all elements matches the sum of `partial_chunk_sum` within floating‐point tolerance. When `n` is not divisible by chunk_size, the last chunk is smaller, and when`chunk_size ≥ n`, only one chunk is processed. Error handling is confirmed for mismatched sizes and zero chunk size via exceptions.


## What you will find in this repository

- **Header Files (`include/`):**  
    Provides declarations for the `vector_sum_chunked` function.

- **Test Files (`test/`):**  
    Contains unit tests for the `vector_sum_chunked` function.

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
│   ├── vector_sum.hpp            # Declaration of the vector_sum function
│   ├── vector_sum_chunked.hpp     # Declaration of the vector_sum_chunked function
├── scripts/                 # Helper scripts
│   ├── buildProject.sh           # Script to build the project from scratch
│   └── destroyProject.sh         # Script to completely clean the project
├── test/                    # Unit tests
│   ├── test_vector_sum.cpp       # Tests for the vector_sum_chunked function
├── CMakeLists.txt           # CMake build configuration file
└── README.md                # Project documentation  
```