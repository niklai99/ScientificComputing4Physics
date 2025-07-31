This page describes, step by step, how the different components of Task 3 work together to generate input vectors, select an algorithm, compute the sum \(d = a\,x + y\), and write the result. 

---

## Generating Vectors

1. **Invoke the executable**  

    From the terminal , use the `run` command associated with the `generateVectors` executable:

    ```bash
    run generateVectors <N> <filename_prefix>
    ```
        
    where

    - `<N>` is the number of elements in each vector and `<filename_prefix>` is the prefix for the output files.
    - `<filename_prefix>` is a path and file‐name prefix (for example, `data/input/vector.txt`).

2. **What happens inside**

    - The program allocates two `std::vector<double>` of length `N`.
    - It fills both vectors with a constant value (set in code).
    - It calls the `write_vector` helper, which:

        1. Examines the extension of `<filename_prefix>` (`.txt`, `.dat`, or .`h5`).
        2. Dispatches to the matching I/O routine (plain‐text, binary, or HDF5).
        3. Writes `x` to `<filename_prefix>_x.<ext>` and `y` to `<filename_prefix>_y.<ext>`.

3. **Result**

    - Two files are created: `<filename_prefix>_x.<ext>` and `<filename_prefix>_y.<ext>`, containing the vectors `x` and `y` respectively.

## Preparing the configuration

Before running the sum, you must create or edit a YAML file (for example, config/config.yml) with these fields:

```yaml linenums="1" title="config/config.yml"
input:
  vector_x: "data/input/vector_N10_x.h5"
  vector_y: "data/input/vector_N10_y.h5"
N: 10
a: 3
output:
    path: "./data/output"
    prefix: "result"
    format: "txt" # or "dat" or "h5"
implementation: "gsl" # Or "default"
```

- `input.vector_x`, `input.vector_y`: paths to the two vectors.
- `N`: number of elements (must match how you generated the vectors).
- `a`: scalar multiplier in the sum.
- `output.path`: directory where the result will be saved.
- `output.prefix`: prefix for the output file.
- `output.format`: format for the output file (can be `txt`, `dat`, or `h5`).
- `implementation`: selects the algorithm (see next section).


## Computing the sum

1. **Invoke the executable**  

    From the terminal, use the `run` command associated with the `vectorSum` executable:

    ```bash
    run vectorSum config/config.yml
    ```

2. **YAML parsing**

    The program uses `yaml-cpp` to read all required parameters from `config.yml`.

3. **Reading input vectors**

    It calls the `read_vector` helper, which checks the file extension and reads the data into two `std::vector<double>` objects.

 4. **Choosing the implementation**

    Based on the `implementation` field in the YAML config, it creates an instance of either `VectorSumDefault` or `VectorSumGSL`. Both derive from `VectorSumInterface`.

5. **Computing the sum**

    The chosen implementation's `compute_sum` method is called with the vectors `x`, `y`, scalar `a`, and an output vector `d`.

    - If `implementation` is `default`, it uses a simple loop to compute \(d = a \cdot x + y\).
    - If `implementation` is `gsl`, it uses `gsl_vector_axpby` for an in-place BLAS-optimized update.

6. **Writing the result**

    The program calls the `write_vector` helper to save the output vector `d` to the specified file in the chosen format.