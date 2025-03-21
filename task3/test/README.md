# Scientific Computing for Physics – Task 3

This project demonstrates a “real” example of vector operations, including vector sum and matrix multiplication, with a focus on modular design and file I/O for scientific computing. In this task, you will:

1. **Generate Vectors:**  

   The `generate_vectors` executable creates two vectors (x and y) of dimension N with constant values (x = 0.1, y = 7.1) and saves them as text files. 

   - **Usage Example:**  
     ```bash
     ./run generate_vectors 10 "./data/input/vector"
     ```  
     This command generates the files:
     - `./data/input/vector_N10_x.dat`
     - `./data/input/vector_N10_y.dat`

2. **Compute Vector Sum:**  

   The `vector_sum` executable reads a YAML configuration file (`config/config.yml`) that specifies the input files, the dimension N, the scalar value a (3.0), and output parameters. It then computes the vector sum, \(d = a \times x + y\), and writes the result to an output file.

   - **Usage Example:**  
     ```bash
     ./run vector_sum "./config/config.yml"
     ```
     The result is written to a file in the output directory as specified in the config file.


### What is Shipped to the Public

The public source distribution should include everything **except** the `build/` directory (which is generated during the build). In other words, ship the following:

- **config/**: Configuration files (e.g., `config.yml`)
- **data/**: Sample or empty directories for input/output files
- **include/**: Header files
- **src/**: Source code files
- **commands/**: The run script template (`run.in`)
- **CMakeLists.txt**: Build configuration
- **README.md**: Documentation

Users will then create their own build directory, configure, build, and run the project using the provided instructions.

## How to Build

1. **Create and Enter the Build Directory:**

   ```bash
   mkdir -p build
   cd build

2. **Generate Build Files with CMake:**

   ```bash
   cmake ..
   ```
3. **Build the Project:**

   ```bash
   make
   ```

## How to Run

The project includes a wrapper script named `run` that is installed into `/usr/local/bin` during the build process. This script allows you to invoke your executables using paths relative to the project root.

- **Example to Generate Vectors:**

  ```bash
  run generate_vectors 10 "./data/input/vector"
  ```

- **Example to Compute Vector Sum:**

  ```bash
  run vector_sum "./config/config.yml"
  ```