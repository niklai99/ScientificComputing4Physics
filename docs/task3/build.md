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

    - To generate vectors:

        ```bash
        run generateVectors 10 "./data/input/vector.h5"
        ```

    - To compute the vector sum:

        ```bash
        run vectorSum "./config/config.yml"
        ```

More details on properly using the executables are provided in the subsequent sections of this documentation. You can skip to the next page if not interested in the build process details. 


## In-Depth Build Process Explanation

After running the build script, a series of automated steps are performed behind the scenes to transform the source code into executable programs. Below is an in-depth explanation of what happens during the build process and how to use the run command.

### Build Process Overview

1. **Build Directory Creation:**

    The build script first checks for the existence of the `build/` directory in the project root. If it doesn’t exist, it creates it. This out-of-source build practice keeps all generated files separate from the source code, which simplifies cleanup and avoids polluting the source tree.

    ```bash linenums="1" title="scripts/buildProject.sh"
    # Determine the project root (the directory where this script is located)
    PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    BUILD_DIR="$PROJECT_ROOT/build"

    echo "Building project in: $BUILD_DIR"

    # Create and enter the build directory.
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    ```

1. **CMake Configuration:**

    - Once inside the `build/` directory, the script runs the command 

        ```bash linenums="11" title="scripts/buildProject.sh"
        echo "Running CMake configuration..."
        # Run CMake configuration; exit if it fails.
        cmake .. || { echo "CMake configuration failed"; exit 1; }
        ```

    - CMake reads the `CMakeLists.txt` file located in the project root. This file specifies the project’s configuration:

        - It sets the C++ standard to C++11 and enables common compiler flags like `-Wall`, `-Wextra`, and `-O3` for optimization.

            ```cmake linenums="1" title="CMakelists.txt"
            set(CMAKE_CXX_STANDARD 11)
            set(CMAKE_CXX_STANDARD_REQUIRED ON)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
            ```

        - It defines where the executables will be placed (specifically, in `build/bin`).

            ```cmake linenums="5" title="CMakelists.txt"
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
            get_filename_component(BIN_DIR_ABS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ABSOLUTE)
            ```

            The absolute path (`BIN_DIR_ABS`) is later used in the run script.

        - It locates and configures external dependencies such as yaml-cpp, HDF5, and GSL.

            ```cmake linenums="8" title="CMakelists.txt"
            find_package(yaml-cpp REQUIRED)
            find_package(HDF5 REQUIRED COMPONENTS CXX)
            include_directories(${HDF5_INCLUDE_DIRS})
            find_package(GSL REQUIRED)
            include_directories(${GSL_INCLUDE_DIRS})
            ```

        - It defines the build targets (executables like `generateVectors` and `vectorSum`) and links them with the required libraries.

            ```cmake linenums="14" title="CMakelists.txt"
            add_executable(generateVectors src/generateVectors.cpp)
            add_executable(vectorSum src/vectorSum.cpp)

            target_link_libraries(generateVectors ${HDF5_LIBRARIES})
            target_link_libraries(vectorSum yaml-cpp ${HDF5_LIBRARIES} GSL::gsl GSL::gslcblas)
            ```

        - It configures a run script from a template (located in `commands/run.in`), replacing placeholders (such as the project root and the absolute path to `build/bin`) with actual values. 

            ```cmake linenums="20" title="CMakelists.txt"
            set(RUN_SCRIPT_TEMPLATE ${PROJECT_SOURCE_DIR}/commands/run.in)
            set(RUN_SCRIPT_OUTPUT ${PROJECT_BINARY_DIR}/commands/run)
            configure_file(${RUN_SCRIPT_TEMPLATE} ${RUN_SCRIPT_OUTPUT} @ONLY)
            execute_process(COMMAND chmod 0755 ${RUN_SCRIPT_OUTPUT})
            add_custom_target(copy_run ALL
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RUN_SCRIPT_OUTPUT} /usr/local/bin/run
                COMMENT "Copying run script to /usr/local/bin"
            )
            ```
        
        - It ensures that both executables depend on the copy_run target, so that the run script is always copied when building the project.
          
            ```cmake linenums="29" title="CMakelists.txt"
            add_dependencies(generateVectors copy_run)
            add_dependencies(vectorSum copy_run)
            ```

1. **Compilation with Make:**

      - After CMake has successfully configured the project, the build script invokes make:

        ```bash linenums="15" title="scripts/buildProject.sh"
        echo "Starting build with make..."
        # Build the project with make; exit if the build fails.
        make || { echo "Build failed"; exit 1; }
        ```

      - Make reads the generated Makefiles (created by CMake) and compiles all source files according to the defined rules. This step builds the executables and places them in the `build/bin` directory.
      - The custom target defined in CMake also ensures that the run script is copied to `/usr/local/bin`, so that it is globally accessible.