# Docker Installation and Setup for Task 3

## Overview

This guide explains how to build and run the Docker container that provides a virtualized environment for Task 3 of the Scientific Computing for Physics Students project. The container is based on AlmaLinux9 and includes all required development tools and libraries (yaml-cpp, HDF5, GSL, etc.), as well as a Python environment managed by Miniconda.

!!! info "Prerequisites"

    - Docker must be installed.
    - Follow the Docker installation instructions in [Task 1](../task1/install_docker.md) before proceeding with Task 3.

## Building the Docker Image

The Dockerfile is located in the `docker/` directory and is named `Dockerfile.alma9`.

1. Open a terminal and navigate to the project root.
2. Run the following command to build the Docker image:

    ```bash
    docker build -t sci-comp-task3 -f docker/Dockerfile.alma9 .
    ```
    
      1. `-t sci-comp-task3` tags the image with the name `sci-comp-task3`.
      2. `-f docker/Dockerfile.alma9` specifies the location of the Dockerfile.
      3. The final `.` sets the build context to the project root.

## Running the Docker Container

Once the image is built, you can start a container interactively with:

```bash
docker run -it -v "$(pwd):/workspace" sci-comp-task3
```

- `-it`: Runs the container in interactive mode with a TTY.
- `-v "$(pwd):/workspace"`: Mounts the current project root into the container at /workspace.
- `sci-comp-task3`: Specifies the image name.

Inside the container, your working directory will be `/workspace` (which is your project root), and all necessary tools and libraries will be available.