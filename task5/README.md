# Task 5 Build and Run Guide

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

## Task 5a

To run the `computeSum` executable, you can use the following command:

```bash
run computeSum
```

The output is fixed and should be:

```
Analytic solution: 0.5

For loop summation:
Computed sum: -0.5

GSL summation:
Computed sum: -0.5

Pairwise summation:
Computed sum: 0

Kahan summation:
Computed sum: -0.5

Neumaier summation:
Computed sum: 0.5


Explanation of results:
- Forloop and GSL: Suffer from catastrophic cancellation due to the overwhelming effect of large numbers over small ones, resulting in a sum of -0.5.
- Pairwise: Groups similar-magnitude numbers together, but in this case, it separates the contributions in a way that cancels out the large terms without preserving the small net offset, returning 0.
- Kahan: Introduces a compensation variable, yet still fails to recover the lost precision when faced with catastrophic cancellation, also yielding -0.5.
- Neumaier: Adjusts the compensation strategy to handle cases where the incoming term is larger than the current sum, successfully recovering the lost digits and producing the correct result, 0.5.
```

### Explanation of Results

1. **Naive (For-loop and GSL) Summation**

    - What happens:

        The summation is performed in the given order:

            - Start with 0.0.
            - Add 1.0  →  Sum becomes 1.0.
            - Add 1×10^16 → The huge value overwhelms the 1.0, so the sum becomes 
            - roughly 1×10^16 (the 1.0 is lost in the precision of floating-point).
            - Add -1×10^16 → The large positive and negative values cancel, leading to 
            - an intermediate sum of 0.0.
            - Add -0.5 → Final sum becomes -0.5.
     
    - Why it fails:

        This is an example of catastrophic cancellation. When adding a tiny number (1.0)
        to a huge number (1×10^16), the tiny contribution is lost due to limited precision.
        Later, when the large terms cancel, the lost contribution (which should have helped
        yield 0.5) is never recovered, and the final result becomes -0.5.

2. **Pairwise Summation**

    - What happens:

        Pairwise summation divides the vector into two groups and sums each separately.

        For example:

            - Group 1: [1.0, 1×10^16] → Sum is approximately 1×10^16.
            - Group 2: [-1×10^16, -0.5] → Sum is approximately -1×10^16.

        When these two group sums are combined, the large numbers cancel exactly to 0,
        effectively losing the small net offset of 0.5.
   
    - Why it fails:

        Although pairwise summation is generally more stable, in this case it still loses
        the small contributions when grouping numbers of vastly different magnitudes, and thus
        the overall net offset (0.5) is not preserved.


3. **Kahan Summation**

    - What happens:

        Kahan’s algorithm introduces a compensation variable to capture the low-order bits 
        that are lost during the addition of floating-point numbers.
    
    - Why it still fails:

        Even with the compensation variable, Kahan’s algorithm struggles when the running 
        sum becomes so dominated by a huge number (1×10^16) that the small additions (1.0 or 
        -0.5) are completely swamped. The algorithm cannot capture enough of the tiny errors 
        when the next term is significantly smaller (or the cancellation is catastrophic).
        As a result, it also ends up returning -0.5.


4. **Neumaier’s Improvement (Improved Kahan–Babuška Algorithm)**

    - What happens:

        Neumaier’s algorithm modifies Kahan’s approach by checking the relative magnitudes of 
        the current sum and the incoming term. If the incoming term has a larger absolute value 
        than the running sum, the algorithm updates the compensation variable accordingly.
    
    - Why it works correctly:

        In our example, when adding the terms 1×10^16 and -1×10^16, the algorithm detects that 
        the relative sizes could lead to cancellation errors and adjusts the compensation variable 
        to capture the lost low-order digits. By applying the correction (adding the compensation 
        back) at the end of the process, Neumaier’s algorithm recovers the small contribution of 
        1.0 that was lost, and the final sum becomes 0.5 as expected.


## Task 5b

To run the DAXPY tests, use the following command:

```bash
run testDaxpy
```

This command will execute the DAXPY test suite with default parameter values:

- **Vector Size (n):** 1_000_000
- **Scalar (a):** 3
- **Iterations (n_iter):** 100

You can also override these defaults by providing command-line arguments. For example:

```bash
run testDaxpy 500000 2.5 50
```

This command will execute the DAXPY test suite with:

- **Vector Size (n):** 500_000
- **Scalar (a):** 2.5
- **Iterations (n_iter):** 50


### Testing the correct result using expected mean and standard deviation

The DAXPY tests are designed to verify that the computed result vector `d` (obtained by performing `d = a * x + y`) is statistically correct. The test suite uses the following approach:

1. **Input generation:**

  - Both vectors x and y are generated with Gaussian random numbers having mean 0 and standard deviation 1.

2. **Expected distribution:**

  - Since `x` and `y` are independent and normally distributed, the linear combination `d = a*x + y` should be normally distributed.
  - **Expected mean:** 0 (because the mean of both x and y is 0).
  - **Expected standard deviation:** sqrt(a^2 + 1). This follows from the property of independent normal distributions where the variances add (variance of `a*x`is `a^2`, and variance of `y` is `1`).

3. **Dynamic tolerances:**

  - The test suite computes the sample mean and RMS of the result vector `d`.
  - Dynamic tolerances are applied based on the size of the vector to account for statistical uncertainties:
    - **Mean tolerance:** Scales approximately as `(std_dev / sqrt(n))`, reflecting the standard error of the mean.
    - **RMS tolerance:** Scales approximately as `(std_dev / sqrt(2*n))` due to the variance of the squared values.
  - These tolerances ensure that larger vectors (which have smaller statistical uncertainties) are held to a tighter criterion than smaller ones.
  
4. **Validation:**:

  - The test validates that the computed mean and RMS of `d` fall within these dynamically computed tolerances.
  - The test passes if the statistical properties of `d` match the expected normal distribution.