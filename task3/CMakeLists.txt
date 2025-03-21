cmake_minimum_required(VERSION 3.10)
project(ScientificComputing4Physics)

# Set C++ standard and common compiler flags.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

# Set the runtime output directory for executables to build/bin.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
get_filename_component(BIN_DIR_ABS ${BIN_DIR} ABSOLUTE)

# Include header files.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find the yaml-cpp package.
find_package(yaml-cpp REQUIRED)

# Find the HDF5 package.
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIRS})

# Find the GSL package.
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

# Define executables.
add_executable(generateVectors src/generateVectors.cpp)
add_executable(vectorSum src/vectorSum.cpp)
target_link_libraries(generateVectors ${HDF5_LIBRARIES})
target_link_libraries(vectorSum yaml-cpp ${HDF5_LIBRARIES} GSL::gsl GSL::gslcblas)

# Configure the run script.
set(RUN_SCRIPT_TEMPLATE ${PROJECT_SOURCE_DIR}/commands/run.in)
set(RUN_SCRIPT_OUTPUT ${PROJECT_BINARY_DIR}/commands/run)
configure_file(${RUN_SCRIPT_TEMPLATE} ${RUN_SCRIPT_OUTPUT} @ONLY)

# Set the run script executable using execute_process.
execute_process(COMMAND chmod 0755 ${RUN_SCRIPT_OUTPUT})

# Copy the run script to /usr/local/bin.
add_custom_target(copy_run ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RUN_SCRIPT_OUTPUT} /usr/local/bin/run
    COMMENT "Copying run script to /usr/local/bin"
)

# Make the overall build depend on the copy_run target.
add_dependencies(generateVectors copy_run)
add_dependencies(vectorSum copy_run)
