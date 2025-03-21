#!/bin/bash
# Build script for the project.
#
# This script performs the following steps:
# 1. Determines the project root based on the script location.
# 2. Creates (if necessary) and enters the build directory.
# 3. Runs the CMake configuration step.
# 4. Invokes make to build the project.

# Ensure the script stops on errors, catches undefined variables
set -euo pipefail
IFS=$'\n\t'

# Determine the project root (directory where this script resides)
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$PROJECT_ROOT/build"

echo "Building project in: $BUILD_DIR"

# Create the build directory if it doesn't exist and enter it.
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Running CMake configuration..."
# Run CMake configuration; exit if it fails.
cmake .. || { echo "CMake configuration failed"; exit 1; }

echo "Starting build with make..."
# Build the project with make; exit if the build fails.
make || { echo "Build failed"; exit 1; }

echo "Build completed successfully."
# Return to the project root.
cd "$PROJECT_ROOT"
