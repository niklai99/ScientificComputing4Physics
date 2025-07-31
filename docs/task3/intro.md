# Introduction

Task 3 demonstrates a more complete and practical implementation of basic software and project design. It is designed both as a learning tool and as a practical framework that integrates multiple components into one cohesive system.

## What the Project Does

The project focuses on two primary operations:

1. **Generate Vectors:**  
   The `generateVectors` executable creates two vectors (x and y) of dimension `N` filled with constant values. These vectors can be saved in either plain text format (".txt" or ".dat") or in HDF5 format (".h5"), depending on the file extension provided in the filename prefix.

2. **Compute Vector Sum:**  
   The `vectorSum` executable reads the vectors from files (using the appropriate I/O method based on the file extension) and computes their sum according to the formula:

    $$
    d = a \cdot x + y,
    $$

   Two implementations are provided for this computation:

   - A default, element-by-element approach.
   - An optimized version that leverages the GNU Scientific Library (GSL).
   
   The specific implementation used is determined at runtime via the configuration file.

## What You Will Find in This Repository

- **Source Code (`src/`):**  
    Contains the C++ source files for generating vectors and computing the vector sum.

- **Header Files (`include/`):**  
    Provides declarations for the vector sum interface, both implementations (default and GSL-based), and helper functions for file I/O and path manipulations.

- **Configuration Files (`config/`):**  
    A YAML file (e.g., `config.yml`) that specifies input file paths, the number of elements (N), the scalar multiplier (a), output options, and the implementation choice ("default" or "gsl").

- **Helper Scripts (`scripts/`):**  
    - `buildProject.sh`: A script to build the project from scratch.
    - `destroyProject.sh`: A script to completely clean the project, removing build artifacts and installed commands.

- **Docker Environment (`docker/`):**  
    A Dockerfile (e.g., `Dockerfile.alma9`) is included to provide a ready-to-use development environment with all required dependencies (yaml-cpp, HDF5, and GSL). This ensures that the project can be built and run consistently across different systems.

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
│   ├── HelperFunctions.hpp      # Common helper functions for file/path operations
│   ├── VectorIO.hpp             # I/O helper functions for reading/writing vectors
│   ├── VectorSumInterface.hpp   # Abstract interface for vector sum computation
│   ├── VectorSumDefault.hpp     # Default (elementwise) vector sum implementation
│   └── VectorSumGSL.hpp         # GSL-based vector sum implementation
├── scripts/                 # Helper scripts
│   ├── buildProject.sh          # Script to build the project from scratch
│   └── destroyProject.sh        # Script to completely clean the project
├── src/                     # Source code files
│   ├── generateVectors.cpp      # Code to generate input vectors (supports txt/dat/h5)
│   └── vectorSum.cpp            # Code to compute vector sum (selects implementation at runtime)
├── CMakeLists.txt           # CMake build configuration file
└── README.md                # Project documentation
```

The detailed documentation for each component is provided in a respective sections of this documentation.