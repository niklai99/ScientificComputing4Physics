# -----------------------------------------------------------------------------
# Minimum required CMake version and project declaration.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(ScientificComputing4PhysicsStudentsTask3)

# -----------------------------------------------------------------------------
# Set the C++ standard to C++11 and add common compiler flags.
# - We require C++11 for compatibility with our code.
# - The flags -Wall and -Wextra enable extra warnings,
#   and -O3 enables aggressive optimizations.
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

# -----------------------------------------------------------------------------
# Set the output directory for the compiled executables.
# - All executables will be placed in the "build/bin" directory.
# - We then obtain the absolute path to this directory for later use (e.g., in the run script).
# -----------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
get_filename_component(BIN_DIR_ABS ${BIN_DIR} ABSOLUTE)

# -----------------------------------------------------------------------------
# Include header directories.
# - We add the project's "include" folder to the search paths for header files.
# -----------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include)

# -----------------------------------------------------------------------------
# Find and configure third-party libraries:
#
# 1. yaml-cpp:
#    - Required for YAML configuration file parsing.
#
# 2. HDF5:
#    - Required for HDF5 I/O (reading and writing vector files in HDF5 format).
#    - We use the C++ API (hence COMPONENTS CXX).
#
# 3. GSL:
#    - Required for the alternative (GSL-based) vector sum implementation.
# -----------------------------------------------------------------------------
find_package(yaml-cpp REQUIRED)

find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIRS})

find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

# -----------------------------------------------------------------------------
# Define the executables:
#
# - generateVectors: generates vector files using either text or HDF5 I/O.
# - vectorSum: computes the vector sum using either the default or GSL implementation.
#
# The target_link_libraries commands link the necessary libraries to each executable.
# -----------------------------------------------------------------------------
add_executable(generateVectors src/generateVectors.cpp)
add_executable(vectorSum src/vectorSum.cpp)
target_link_libraries(generateVectors ${HDF5_LIBRARIES})
target_link_libraries(vectorSum yaml-cpp ${HDF5_LIBRARIES} GSL::gsl GSL::gslcblas)

# -----------------------------------------------------------------------------
# Configure the run script:
#
# - The run script template is located in the "commands" directory of the project.
# - We generate a configured run script in the build directory ("build/commands/run")
#   with placeholders replaced (e.g., PROJECT_SOURCE_DIR and BIN_DIR_ABS).
# - This script is later copied to /usr/local/bin to allow easy execution.
# -----------------------------------------------------------------------------
set(RUN_SCRIPT_TEMPLATE ${PROJECT_SOURCE_DIR}/commands/run.in)
set(RUN_SCRIPT_OUTPUT ${PROJECT_BINARY_DIR}/commands/run)
configure_file(${RUN_SCRIPT_TEMPLATE} ${RUN_SCRIPT_OUTPUT} @ONLY)

# -----------------------------------------------------------------------------
# Set the run script as executable using an external command.
# -----------------------------------------------------------------------------
execute_process(COMMAND chmod 0755 ${RUN_SCRIPT_OUTPUT})

# -----------------------------------------------------------------------------
# Create a custom target "copy_run" that automatically copies the run script
# to /usr/local/bin after building.
# - This allows the user to run "run" from anywhere.
# - Note: This step requires the user to have appropriate permissions.
# -----------------------------------------------------------------------------
add_custom_target(copy_run ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RUN_SCRIPT_OUTPUT} /usr/local/bin/run
    COMMENT "Copying run script to /usr/local/bin"
)

# -----------------------------------------------------------------------------
# Ensure that both executables depend on the copy_run target,
# so that the run script is always copied when building the project.
# -----------------------------------------------------------------------------
add_dependencies(generateVectors copy_run)
add_dependencies(vectorSum copy_run)
