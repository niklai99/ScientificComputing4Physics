# Task 4 Build and Run Guide

## How to Build the Project

Assuming you already have the project available locally, then execute the build script from the project root directory:

```bash
bash ./scripts/buildProject.sh
```

This script performs the following steps:

- **Creates a Build Directory:**

  It checks for the existence of the `build/` directory and creates it if necessary.

- **CMake Configuration:**

  It changes into the `build/` directory and runs `cmake ..` to generate the build system. CMake reads the `CMakeLists.txt` file (which sets the C++ standard, compiler flags, output directories, and dependency configuration) and configures the project.

- **Compilation with Make:**

  It then calls `make` to compile the source code into executables, which are placed in `build/bin`.

- **Run Script Configuration:**

  During the CMake process, a run script is generated from the template in `commands/run.in` and copied to `/usr/local/bin` for easy access.

- **Return to Project Root:**

  After the build completes, the script returns you to the project root.



## How to Run the Executables

After building, you can run the executables using the globally installed `run` wrapper script. This script simplifies execution by automatically setting the correct working directory and locating the executables.


### Examples

- **Save Sampled Function and Compute Integral (compiled language):**

    ```bash
    run computeIntegral <N> <x_inf> <x_sup> 
    ```

    or 

    ```bash
    run computeIntegral <path/to/config.yml>
    ```

- **Read Sampled Function and Compute Integral (interpreted language):**

    ```bash
    run compute_integral --N <N> --precision <precision> 
    ```

