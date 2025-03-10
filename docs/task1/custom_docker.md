# Setting up the AlmaLinux9 Container for Python and C/C++ Development using a Dockerfile

In this section, instead of simply pulling the AlmaLinux9 image, we will create a custom Dockerfile that:

- Installs essential development tools (e.g., GCC, Make, and CMake) for compiling C++ code.
- Installs Miniconda so you can easily manage Python packages.
- Sets up a working directory (e.g., `/workspace`) and marks it as a volume. This enables you to mount a local folder to persist data even after the container is shut down.

### Creating the Dockerfile

Create a file named `Dockerfile.alma9` with the following content:
    
```dockerfile
# Use AlmaLinux9 as the base image
FROM almalinux:9

# Set environment variables for consistent locale settings
ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# Update system packages and install essential development tools
RUN dnf -y update && \
    dnf -y install bash gcc gcc-c++ make cmake curl --allowerasing && \
    dnf clean all

# Install Miniconda (ARM version for aarch64) to manage Python packages
RUN curl -fsSL https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-aarch64.sh -o /tmp/miniconda.sh && \
    bash /tmp/miniconda.sh -b -p /opt/conda && \
    rm /tmp/miniconda.sh

# Add Miniconda to the PATH
ENV PATH="/opt/conda/bin:${PATH}"

# Create and set the working directory (e.g., /workspace)
RUN mkdir -p /workspace
WORKDIR /workspace

# Mark /workspace as a volume so that local data can be persisted
VOLUME ["/workspace"]

# Define the default command to start an interactive bash shell
CMD ["/bin/bash"]
```

The RUN command updates the system and installs essential packages:

- `gcc` and `gcc-c++`: Provide the compilers needed to build C++ code.
- `make` and `cmake`: Tools for automating builds.
- `curl`: Needed to download the Miniconda installer.

Miniconda is installed using the ARM-specific installer script [Miniconda3-latest-Linux-aarch64.sh](https://repo.anaconda.com/miniconda/). This provides a lightweight Python environment that you can be easily extended with additional Python packages.

!!! info 
    If you need a specific Python version or more libraries installed by default, add them to this Dockerfile or run them inside the container using `conda install <package_name>`.

!!! note
    The Dockerfile creates a `/workspace` directory, sets it as the current working directory, and uses a `VOLUME` instruction to indicate that this directory is intended for persistent storage. When you run the container, you can mount a local directory to `/workspace` so that any data created or modified there is preserved even after the container stops.

### Building and Running the Image 

1. **Build the Image**

    Open your Terminal and run:

    ```bash
    docker build -t alma9 -f Dockerfile.alma9 .
    ```

    This command builds the image and tags it as `alma9`. 

    !!! warning "DO NOT forget the dot at the end of the command!"
        The single dot (.) at the end of the command sets the [build context](https://docs.docker.com/build/concepts/context/) to the current directory. This means that the build expects to find the Dockerfile in the directory where the command is invoked. If the file is not there, the build fails. 
    
2. **Run the Container**

    Once the image is built, start a container interactively with:

    ```bash
    docker run -it -v /path/to/local/workspace:/workspace --name <process_name> alma9
    ```

    This command creates a container (naming it `<process_name>`) from your custom image and mounts your host’s `/path/to/local/workspace` to the container’s `/workspace` volume. Any files you create or modify inside `/workspace` will be stored on your host machine. It also works the other way around: any files in `/path/to/local/workspace` will be accessible from inside the container.


## Setting up the Miniconda Environment

Once inside the container, you can create and manage Python environments using Miniconda.

1. **Create a New Conda Environment**

    Run the following command to create a new Conda environment named `python_env`:

    ```bash
    conda create -n python_env python numpy pandas matplotlib scipy
    ```

    This creates a new environment with the latest versions of Python. It also installs the popular scientific computing libraries NumPy, Pandas, Matplotlib, and SciPy.

2. **Activate the Conda Environment**
   
    Activate the environment with:

    ```bash
    conda activate python_env
    ```

    The prompt should change to show the active environment.


<!-- !!! warning

    You may need to run 
    
    ```bash
    conda init
    ```

    before activating the environment. This command initializes the shell to use Conda.
     -->