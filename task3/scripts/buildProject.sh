#!/bin/bash
# Build script for the project

# Exit immediately if a command exits with a non-zero status,
# if any unset variable is used, and if any command in a pipeline fails.
set -euo pipefail
IFS=$'\n\t'

# Determine the project root (the directory where this script is located)
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR="$PROJECT_ROOT/build"

echo "Building project in: $BUILD_DIR"

# Create and enter the build directory.
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Running CMake configuration..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

echo "Starting build with make..."
make || { echo "Build failed"; exit 1; }

echo "Build completed successfully."
cd "$PROJECT_ROOT"
