# Task 6 Build and Run Guide

## How to Build the Project

Assuming you already have the project available locally, then execute the build script from the project root directory:

```bash
bash ./scripts/buildProject.sh
```

This script performs the following steps:

- **Creates a Build Directory:**

  It checks for the existence of the `build/` directory and creates it if necessary.

- **CMake Configuration:**

  It changes into the `build/` directory and runs `cmake ..` to generate the build system. CMake reads the `CMakeLists.txt` file (which sets the C++ standard, compiler flags, output directories, and dependency configuration) and configures the project.

- **Compilation with Make:**

  It then calls `make` to compile the source code into executables, which are placed in `build/bin`.

- **Run Script Configuration:**

  During the CMake process, a run script is generated from the template in `commands/run.in` and copied to `/usr/local/bin` for easy access.

- **Return to Project Root:**

  After the build completes, the script returns you to the project root.

After building, you can run the executables using the globally installed `run` wrapper script. This script simplifies execution by automatically setting the correct working directory and locating the executables.

## Task 6

To run the `task06` executable, you can use the following command:

```bash
run task06
```

The output should look like this:

```plaintext
=== c2c_trim round‑trip errors ===
  RMSE(abs) = 2.74066e-14
  MedRSE(abs)= 1.58103e-14
  RMSE(rel) = 1.54794e-11
  MedRSE(rel)= 1.39873e-14

C[0][0] = (1.00119e+06,0)  (≈ sum of A)

=== r2c round‑trip errors ===
  RMSE(abs) = 2.822e-14
  MedRSE(abs)= 1.66533e-14
  RMSE(rel) = 1.62722e-11
  MedRSE(rel)= 1.49467e-14

R[0][0] = (1.00119e+06,0)  (DC term again)

```

### Answer to Task 6 questions

1. **Are you reaching machine precision when reconstructing the original data?**

    A “perfect” machine‐precision result is on the order of IEEE double‑precision $\epsilon \sim 2 \times 10^{-16}$. The absolute RMSE of a few $10^{-14}$ is only two orders of magnitude above $\epsilon$, which is exactly what one would expect if you accumulate rounding error over $\sim 10^6$ points (roughly $\sqrt{N}\cdot\epsilon\simeq 10^3\cdot10^{-16}\sim10^{-13}$). The relative RMSE of $\sim 10^{-11}$ likewise reflects that we are doing `O(N log N)` operations, each introducing O($\epsilon$) error.

    Bottom line: YEs, we are essentially hitting machine precision. The tiny excess over $\epsilon$ comes from doing millions of floating‑point ops (every butterfly, every complex multiply/add).