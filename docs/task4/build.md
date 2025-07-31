# How to Build the Project

## Step-by-Step Instructions

Assuming you already have the project available locally and your Docker container is running with `/workspace` set as the project root, follow these steps to build the project:

1. **Navigate to the Project Root:**

    Ensure you are in the project root directory (i.e., `/workspace`). You can verify this by running:

    ```bash
    pwd
    ```

    It should display `/workspace` (the project root).

2. **Run the Build Script:**

    From the project root, execute the build script to configure and compile the project:

    ```bash
    bash ./scripts/buildProject.sh
    ```

    This script will:

    - Create a build directory 
    - Change into the build directory.
    - Run CMake to generate the build system.
    - Invoke make to compile the source code into executables.
    - Return you to the project root once the build is complete.
  
3. **Run the Executables:**

    You can now run the executables using the `run` command from anywhere. For example:

    - **Save sampled function and compute integral (compiled language):**

        ```bash
        run computeIntegral <N> <x_inf> <x_sup> 
        ```

        or 

        ```bash
        run computeIntegral <path/to/config.yml>
        ```

    - **Read sampled function and compute integral (interpreted language):**

        ```bash
        run compute_integral --N <N> --precision <precision> 
        ```

    More details on properly using the executables are provided in the subsequent sections of this documentation. 