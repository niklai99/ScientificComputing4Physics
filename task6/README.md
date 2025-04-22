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

    Bottom line: Yes, we are essentially hitting machine precision. The tiny excess over $\epsilon$ comes from doing millions of floating‑point ops (every butterfly, every complex multiply/add).

2. **What is the value of C[0,0] or R[0,0]? Can you guess its meaning?**

    That’s the DC component (the zero‐frequency bin) of the transform. In an un‐normalized forward FFT,

    $$C[0,0] = \sum_{i,j} A[i,j]$$

    and similarly for the real‐to‐complex version. In other words, it’s the total “mass” (or N×mean) of your input matrix.

## Task 6 bonus

When your input matrix **A** is real, its 2D FFT **C = FFT2D(A)** obeys **Hermitian symmetry**:

$$
C[i,j] = \Bar{C[(M–i) \mod M, (N–j) \mod N]}
$$

so you only need to store the first ⌊N/2⌋+1 columns of each row; the rest can be filled in by reflecting and conjugating. In other words, every frequency coefficient in the missing half of the spectrum is just the complex conjugate of one you already have, but “reflected” across the origin in frequency space.

### Workflow

1. **Generate test matrix**:

    - Create a 6x6 real matrix A with entries sampled from a Normal distribution N(1,1).

2. **Compute full spectrum C**:

    - Promote A to complex: all imaginary parts set to zero.
    - Compute the full 2D FFT of A using the C2C FFT code.

3. **Compute half spectrum R**:

    - Compute the half-spectrum from A using the R2C FFT code.

4. **Reconstruct C from R**:

    - Let M′ = R.rows(), N′ = 2*(R.cols()–1).
    - Allocate an M′×N′ complex matrix C_rec.
    - Copy the non‑redundant block:

    ```cpp
    for (i = 0; i < M′; ++i)
        for (j = 0; j < R.cols(); ++j)
            C_rec[i][j] = R[i][j];
    ```

    - Fill the remaining columns by Hermitian symmetry:

    ```cpp
    for (i = 0; i < M′; ++i)
        for (j = R.cols(); j < N′; ++j) {
            i_sym = (M′ - i) % M′;
            j_sym = (N′ - j) % N′;
            C_rec[i][j] = std::conj( R[i_sym][j_sym] );
    }
    ```

5. **Verify equality**:

    - Compare `C_full` vs `C_rec` element‑wise.


### Example run

```bash
$ run task06_bonus
```

```plaintext
Max abs(C_full - C_from_R) = 7.10543e-15
✔ Bonus: reconstructed spectrum matches within tol = 1e-12
```