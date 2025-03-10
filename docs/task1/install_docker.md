# Docker Installation and AlmaLinux9 Container Setup on macOS (Apple Silicon)

This guide provides step-by-step instructions to install Docker Desktop on macOS devices equipped with Apple Silicon (M1, M2, etc.) and to set up a reproducible containerized environment using AlmaLinux9.


## 1. Installing Docker on macOS (Apple Silicon) 

### Prerequisites 


- A Mac with an Apple Silicon chip (M1, M2, etc.)
- macOS (the latest version is always recommended)
- An active Internet connection

### Installation Steps 

!!! question "Do you have Rosetta installed?"

    Before we get into the Docker installation, make sure you have Rosetta installed in your machine.
    
    To check whether Rosetta is installed, open a terminal and run 
    
    ```bash
    lsbom -f /Library/Apple/System/Library/Receipts/com.apple.pkg.RosettaUpdateAuto.bom
    ```
    
    You should see at least three files listed as output of the command. If not, it is likely that Rosetta is not installed. Then, install Rosetta with:
    
    ```bash
    softwareupdate --install-rosetta
    ```

    !!! info 

        Rosetta 2 is Apple's dynamic binary translator that enables applications compiled for Intel’s x86_64 architecture to run on Apple Silicon (arm64) devices. While Docker Desktop is largely available as a native ARM application on these systems, some components—such as certain command-line utilities or legacy container images—may still be provided only in x86_64 format (e.g.,  amd64). Installing Rosetta 2 ensures that these non-native binaries are automatically translated and executed correctly, thereby maintaining full compatibility and ensuring a smooth installation and operational experience on your Mac.


1. **Download Docker Desktop** 
      - Visit the [Docker Desktop setup page](https://docs.docker.com/desktop/setup/install/mac-install/) and download the version for Apple Silicon.

2. **Install Docker Desktop**
      - Open the downloaded `.dmg` file.
      - Drag the `Docker.app` icon into the Applications folder.
      - Launch Docker Desktop from the Applications folder.
      - Follow the on-screen instructions and accept the license agreement.
        - A step-by-step installation guide is detailed in the [Docker Desktop setup page](https://docs.docker.com/desktop/setup/install/mac-install/#install-and-run-docker-desktop-on-mac) for macOS.


3. **Verify the Installation** 
    - With the Docker Desktop app running, open the Terminal and run:

        ```bash
        docker --version
        ```

    - You should see output indicating the Docker version installed:

        ```
        Client:
            Version:           27.5.1
            API version:       1.47
            Go version:        go1.22.11
            Git commit:        9f9e405
            Built:             Wed Jan 22 13:37:19 2025
            OS/Arch:           darwin/arm64
            Context:           desktop-linux

        Server: Docker Desktop 4.38.0 (181591)
            Engine:
            Version:          27.5.1
            API version:      1.47 (minimum version 1.24)
            Go version:       go1.22.11
            Git commit:       4c9b3b0
            Built:            Wed Jan 22 13:41:25 2025
            OS/Arch:          linux/arm64
            Experimental:     false
            containerd:
            Version:          1.7.25
            GitCommit:        bcc810d6b9066471b0b6fa75f557a15a1cbf31bb
            runc:
            Version:          1.1.12
            GitCommit:        v1.1.12-0-g51d5e946
            docker-init:
            Version:          0.19.0
            GitCommit:        de40ad0
        ```

## 2. Starting with AlmaLinux9

AlmaLinux9 is available as a pre-built image on [Docker Hub](https://hub.docker.com/). You can quickly begin using it by pulling the image and running a container interactively.

### Step 1: Pull the AlmaLinux image

To pull the [AlmaLinux9](https://hub.docker.com/r/almalinux/9-base) image from Docker Hub, open your Terminal and execute the following command:

```bash
docker pull almalinux:9
```

The output you should get is:

```
9: Pulling from library/almalinux
5286574881d2: Pull complete 
Digest: sha256:6d78b43b103e7ca90c43a790e73bdd421cec5ae0269b6051ef6836f4070b7476
Status: Downloaded newer image for almalinux:9
docker.io/library/almalinux:9
```

### Step 2: Run an Interactive Container

Once the image is available locally, start a new container by running:

```bash
docker run -it almalinux:9 /bin/bash
```

- **Flags:**
    - `-i` (interactive): Keeps the standard input open, allowing you to interact with the container.
    - `-t` (tty): Allocates a pseudo-terminal, so you receive a shell prompt.

- **Command Override:** 
    - The command `/bin/bash` is passed to override the default command in the image, so you are dropped directly into a bash shell.


### Step 3: Verify Your Container Environment

Within the container, you can verify the operating system by running:

```bash
cat /etc/os-release
```

This command displays details about the AlmaLinux distribution, confirming that you are running AlmaLinux9.

For example:

```
NAME="AlmaLinux"
VERSION="9.5 (Teal Serval)"
ID="almalinux"
ID_LIKE="rhel centos fedora"
VERSION_ID="9.5"
PLATFORM_ID="platform:el9"
PRETTY_NAME="AlmaLinux 9.5 (Teal Serval)"
ANSI_COLOR="0;34"
LOGO="fedora-logo-icon"
CPE_NAME="cpe:/o:almalinux:almalinux:9::baseos"
HOME_URL="https://almalinux.org/"
DOCUMENTATION_URL="https://wiki.almalinux.org/"
BUG_REPORT_URL="https://bugs.almalinux.org/"

ALMALINUX_MANTISBT_PROJECT="AlmaLinux-9"
ALMALINUX_MANTISBT_PROJECT_VERSION="9.5"
REDHAT_SUPPORT_PRODUCT="AlmaLinux"
REDHAT_SUPPORT_PRODUCT_VERSION="9.5"
SUPPORT_END=2032-06-01
```

### Step 4: Exit the Container

When you are finished working inside an interactive container, you can exit the container gracefully.


At the bash prompt inside the container, simply type:

```bash
exit
```

Then press <kbd>Enter</kbd>. This command terminates the shell session, which in turn stops the container.

Alternatively, you can press <kbd>control</kbd><kbd>D</kbd> on your keyboard. This shortcut sends an end-of-file (EOF) signal, which also terminates the interactive shell session.