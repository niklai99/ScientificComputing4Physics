# Introduction to Task 7

In Task 7, we write and document a suite of unit tests for the DAXPY-inspired function `vector_sum`

```cpp title="vector_sum.hpp" 
void vector_sum(double a,
                const std::vector<double>& x,
                const std::vector<double>& y,
                std::vector<double>& d);
```

The goals of this task are:

1. Verify basic functionality:  Ensure that vector_sum produces correct results for normal inputs.
2. Test edge cases:  Confirm that the function handles empty vectors and throws an exception when input sizes mismatch.

## Testing strategy

We use a simple `main`-based test driver (`test_vector_sum.cpp`) with assert statements to enforce expected behavior. Three categories of tests are included:

### Basic functionality tests

We check that for

```cpp title="test_vector_sum.cpp" 
double a = 2.0;
std::vector<double> x{1.0, 2.0, 3.0};
std::vector<double> y{4.0, 5.0, 6.0};
std::vector<double> d;
vector_sum(a, x, y, d);
```

the output vector d has size 3 and values {6.0, 9.0, 12.0} via:

```cpp title="test_vector_sum.cpp"
assert(d.size() == x.size());
assert(d[0] == 6.0);
assert(d[1] == 9.0);
assert(d[2] == 12.0);
```

### Empty vectors tests

We verify that calling

```cpp title="test_vector_sum.cpp"
vector_sum(5.0, std::vector<double>{}, std::vector<double>{}, d);
```

leaves the result d empty:

```cpp title="test_vector_sum.cpp"
assert(d.empty());
```

### Mismatched sizes tests

We ensure that passing vectors of unequal lengths throws an exception:

```cpp title="test_vector_sum.cpp"
std::vector<double> x{1.0};
std::vector<double> y{1.0, 2.0};
bool thrown = false;
try {
    vector_sum(1.0, x, y, d);
} catch (const std::invalid_argument&) {
    thrown = true;
}
assert(thrown);
```


## What you will find in this repository

- **Header Files (`include/`):**  
    Provides declarations for the `vector_sum` function.

- **Test Files (`test/`):**  
    Contains unit tests for the `vector_sum` function.

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
├── scripts/                 # Helper scripts
│   ├── buildProject.sh           # Script to build the project from scratch
│   └── destroyProject.sh         # Script to completely clean the project
├── test/                    # Unit tests
│   ├── test_vector_sum.cpp       # Tests for the vector_sum function
├── CMakeLists.txt           # CMake build configuration file
└── README.md                # Project documentation  
```