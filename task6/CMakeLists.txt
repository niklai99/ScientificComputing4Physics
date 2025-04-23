# -----------------------------------------------------------------------------
# Minimum required CMake version and project declaration.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(ScientificComputing4PhysicsStudentsTask6)

# -----------------------------------------------------------------------------
# Set the C++ standard to C++11 and add common compiler flags.
# - We require C++11 for compatibility with our code.
# - The flags -Wall and -Wextra enable extra warnings,
#   and -O3 enables aggressive optimizations.
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O0")

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

# # -----------------------------------------------------------------------------
# # Find and configure third-party libraries:
# #
# # 1. xxx
# # -----------------------------------------------------------------------------
# find_package(xxx REQUIRED)
# include_directories(${xxx_INCLUDE_DIRS})

# -----------------------------------------------------------------------------
# Build the custom libraries into static libraries:
#
# -----------------------------------------------------------------------------
add_library(fft_lib STATIC src/FFT.cpp)
target_include_directories(fft_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)



# -----------------------------------------------------------------------------
# Define the executables:
#
# The target_link_libraries commands link the necessary libraries to each executable.
# -----------------------------------------------------------------------------
add_executable(test_fft_pow2   test/test_fft_next_power_of_two.cpp)
target_link_libraries(test_fft_pow2 PRIVATE fft_lib)

add_executable(test_fft1d      test/test_fft1d.cpp)
target_link_libraries(test_fft1d PRIVATE fft_lib)

add_executable(test_fft2d_c2c  test/test_fft2d_c2c.cpp)
target_link_libraries(test_fft2d_c2c PRIVATE fft_lib)

add_executable(test_fft2d_c2c_trim  test/test_fft2d_c2c_trim.cpp)
target_link_libraries(test_fft2d_c2c_trim PRIVATE fft_lib)

add_executable(test_fft2d_r2c_trim  test/test_fft2d_r2c_trim.cpp)
target_link_libraries(test_fft2d_r2c_trim PRIVATE fft_lib)

add_executable(test_fft2d_r2c_reconstruct_full test/test_fft2d_r2c_reconstruct_full.cpp)
target_link_libraries(test_fft2d_r2c_reconstruct_full PRIVATE fft_lib)

add_executable(task06 src/task06.cpp)
target_link_libraries(task06 PRIVATE fft_lib)

add_executable(task06_bonus src/task06_bonus.cpp)
target_link_libraries(task06_bonus PRIVATE fft_lib)


# target_include_directories(task06 PRIVATE ${PROJECT_SOURCE_DIR}/include)


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
add_dependencies(test_fft_pow2 copy_run)
add_dependencies(test_fft1d copy_run)
add_dependencies(test_fft2d_c2c copy_run)
add_dependencies(test_fft2d_c2c_trim copy_run)
add_dependencies(test_fft2d_r2c_trim copy_run)
add_dependencies(test_fft2d_r2c_reconstruct_full copy_run)
add_dependencies(task06 copy_run)
add_dependencies(task06_bonus copy_run)

# -----------------------------------------------------------------------------
# Set the CMake export compile commands option to ON.
# - This generates a compile_commands.json file in the build directory,
#   which can be used by various tools (e.g., IDEs, linters).
# - This file contains information about how each source file is compiled,
#   including compiler flags, include paths, and more.
# ------------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
