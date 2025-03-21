#!/bin/bash
# This script completely cleans the project so that you can rebuild from scratch.
# It removes the build directory and the installed run script.

set -euo pipefail
IFS=$'\n\t'

# Determine the project root (directory where this script is located)
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Project root: $PROJECT_ROOT"

# Prompt the user for confirmation.
read -p "This will delete the build directory and remove the run script from /usr/local/bin. Are you sure? (y/n): " answer
if [[ "$answer" != "y" && "$answer" != "Y" ]]; then
    echo "Aborted."
    exit 0
fi

# Remove the build directory if it exists.
if [ -d "$PROJECT_ROOT/build" ]; then
    echo "Deleting build directory..."
    rm -rf "$PROJECT_ROOT/build"
else
    echo "Build directory does not exist."
fi

# Remove the run script from /usr/local/bin if it exists.
if [ -f "/usr/local/bin/run" ]; then
    echo "Removing /usr/local/bin/run..."
    rm -f /usr/local/bin/run
else
    echo "/usr/local/bin/run not found."
fi

echo "Project cleaned successfully. You can now rebuild from scratch."
