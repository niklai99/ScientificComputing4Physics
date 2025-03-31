# -----------------------------------------------------------------------------
# Minimum required CMake version and project declaration.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(ScientificComputing4PhysicsStudentsTask4)

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
# -----------------------------------------------------------------------------
find_package(yaml-cpp REQUIRED)

# -----------------------------------------------------------------------------
# Define the executables:
#
# The target_link_libraries commands link the necessary libraries to each executable.
# -----------------------------------------------------------------------------
add_executable(computeIntegral src/computeIntegral.cpp)
target_link_libraries(computeIntegral yaml-cpp)

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
# Ensure that executables depend on the copy_run target,
# so that the run script is always copied when building the project.
# -----------------------------------------------------------------------------
add_dependencies(computeIntegral copy_run)


# Copy the Python script to the bin directory and set it as executable.
add_custom_target(compute_integral ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/python/compute_integral.py ${BIN_DIR_ABS}/compute_integral
    COMMAND chmod 0755 ${BIN_DIR_ABS}/compute_integral
    COMMENT "Copying and setting executable permission for Python integration script to ${BIN_DIR_ABS}"
)



# Ensure that the main executable depends on the python script copy target.
add_dependencies(computeIntegral compute_integral)


# -----------------------------------------------------------------------------
# Set the CMake export compile commands option to ON.
# - This generates a compile_commands.json file in the build directory,
#   which can be used by various tools (e.g., IDEs, linters).
# - This file contains information about how each source file is compiled,
#   including compiler flags, include paths, and more.
# ------------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
