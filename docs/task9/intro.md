# Introduction to Task 9

In Task 9, we parallelize the core DAXPY operation

$$
d_i = a \cdot x_i + y_i
$$

using two complementary CPU‐side models: OpenMP for shared‐memory threading and MPI for distributed‐memory message passing. We then measure and compare each parallel version’s execution time against the baseline serial implementation.

## OpenMP vs MPI

| Aspect                | OpenMP                                        | MPI                                                               |
| --------------------- | --------------------------------------------- | ----------------------------------------------------------------- |
| **Memory Model**      | Shared—threads access the same address space. | Distributed—each rank has a separate memory space.                |
| **Parallelism Scope** | Loop‐level/task‐level within single node.     | Process‐level across nodes (or processes).                        |
| **Overhead**          | Thread spawn/synchronization cost.            | Process launch and communication (e.g., broadcast) cost.          |
| **Use Case**          | Multi‐core scaling on one machine.            | Multi‐node scaling or when explicit data‐partitioning is desired. |


## Implementation highlights

- **OpenMP**: In `vector_sum_omp.hpp`, the core loop is decorated with:
  
    ```cpp title="vector_sum_omp.hpp"
    #pragma omp parallel for
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = a * x[i] + y[i];
    }
    ```

    This simple directive forks threads to split the index range, and at the end of the loop, threads synchronize automatically. In `test_vector_sum_omp.cpp`, we time the serial and OpenMP runs back‐to‐back and confirm `d_ser == d_omp` before reporting:

    ```cpp title="test_vector_sum_omp.cpp"
    auto t0 = std::chrono::high_resolution_clock::now();
    vector_sum(a, x, y, d_ser);
    auto t1 = std::chrono::high_resolution_clock::now();
    vector_sum_omp(a, x, y, d_omp);
    auto t2 = std::chrono::high_resolution_clock::now();

    assert(d_ser == d_omp);
    ```

- **MPI**: The MPI implementation lives in `vector_sum_mpi.hpp`. The test driver `test_vector_sum_mpi.cpp` initializes MPI, then:
    1. Root process (rank 0) allocates and fills `x` and `y`.
    2. Broadcast both vectors to all ranks:
   
        ```cpp title="test_vector_sum_mpi.cpp"
        MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(y.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        ```

    3. Each rank calls `vector_sum_mpi(a, x, y, d_mpi)` on its local copy.
    4. Root measures serial vs. MPI time, asserts equality, and prints:

        ```cpp title="test_vector_sum_mpi.cpp"
        auto t0 = std::chrono::high_resolution_clock::now();
        vector_sum(a, x, y, d_ser);
        auto t1 = std::chrono::high_resolution_clock::now();
        vector_sum_mpi(a, x, y, d_mpi);
        auto t2 = std::chrono::high_resolution_clock::now();

        assert(d_ser == d_mpi);
        ```


## What you will find in this repository

- **Header Files (`include/`):**  
    Provides declarations for the `vector_sum_omp` and `vector_sum_mpi` functions.

- **Test Files (`test/`):**  
    Contains unit tests for the `vector_sum_omp` and `vector_sum_mpi` functions.

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
│   ├── vector_sum_chunked.hpp    # Declaration of the vector_sum_chunked function
│   ├── vector_sum_mpi.hpp        # Declaration of the vector_sum_mpi function
│   ├── vector_sum_omp.hpp        # Declaration of the vector_sum_omp function
│   ├── vector_sum.hpp            # Declaration of the vector_sum function
├── scripts/                 # Helper scripts
│   ├── buildProject.sh           # Script to build the project from scratch
│   └── destroyProject.sh         # Script to completely clean the project
├── test/                    # Unit tests
│   ├── test_vector_sum_mpi.cpp   # Tests for the vector_sum_mpi function
│   ├── test_vector_sum_omp.cpp   # Tests for the vector_sum_omp function
│   ├── test_vector_sum.cpp       # Tests for the vector_sum_chunked function
├── CMakeLists.txt           # CMake build configuration file
└── README.md                # Project documentation  
```