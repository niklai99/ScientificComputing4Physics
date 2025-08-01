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

    - **Compute the sum of elements in a vector using different methods:**

        ```bash
        run computeSum
        ```

    - **Perform daxpy operation (d=a*x + y) with random vectors:**

        ```bash
        run testDaxpy
        ```

        or, to override default parameters:

        ```bash
        run testDaxpy <N> <a> <n_iter>
        ```



    More details on properly using the executables are provided in the subsequent sections of this documentation. 