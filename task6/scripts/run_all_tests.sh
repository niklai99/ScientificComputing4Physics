#!/usr/bin/env bash
#
# scripts/run_all_tests.sh
# ------------------------
# Discovers and runs all test executables in build/bin whose names start with "test_".
# Usage: bash ./scripts/run_all_tests.sh
#
set -euo pipefail
IFS=$'\n\t'

# Locate the project root (assumes this script lives in scripts/)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
BUILD_BIN="$PROJECT_ROOT/build/bin"

if [[ ! -d "$BUILD_BIN" ]]; then
  echo "Error: build/bin not found.  Please run 'bash scripts/buildProject.sh' first."
  exit 1
fi

echo "=== Running all tests in $BUILD_BIN ==="
for exe in "$BUILD_BIN"/test_*; do
  if [[ -x "$exe" && ! -d "$exe" ]]; then
    name=$(basename "$exe")
    echo
    echo ">>> $name <<<"
    run "$name"
    status=$?
    if [[ $status -ne 0 ]]; then
      echo "✗ $name FAILED (exit code $status)"
      exit $status
    else
      echo "✓ $name passed"
    fi
  fi
done

echo
echo "All tests passed successfully!"
