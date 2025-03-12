# Testing the AlmaLinux9 Container with Simple Python and C++ Code

To ensure that your AlmaLinux9 container works correctly, we will test it with simple Python and C++ scripts.

!!! example "Small Assignment"
    Suppose you have:
    
    - \( a = 3 \): a real-valued scalar
    - \( x = (1,1,1,\dots,1) \): a real-valued vector of dimension \(N = 20\)
    - \( y = (4,4,4,\dots,4) \): another real-valued vector of dimension \(N = 20\)

    Write a program that calculates \( z = a \cdot x + y \) and prints its values to a text file.


## Python code

Find the Python code in GitHub [here](https://github.com/niklai99/ScientificComputing4Physics/blob/main/task1/test/scalar_prod_python.py).

```python linenums="1" title="scalar_prod_python.py"
import os
import numpy as np

# Define the output file
OUTPUT_DIR = "output"
OUTPUT_FILE = "out_python.txt"
path_to_output = os.path.join(OUTPUT_DIR, OUTPUT_FILE)
    
# Define the input variables
a = 3
x = np.full(20, 1)
y = np.full(20, 4)

# Calculate the result z = a*x + y
z = a * x + y # z is a vector of dimension N = 20 with all elements equal to 7

# Write the result to a text file
with open(path_to_output, "w") as f:
    f.write("z = " + str(z))
```

### Running the Python script

1. Make sure your conda environment is activated (if you’re using Miniconda):

    ```bash
    conda activate python_env
    ```

2. Install NumPy (if not already installed):

    ```bash
    pip install numpy
    ```

3. Run the Python script:

    ```bash
    python scalar_prod_python.py
    ```

4. Check the output file `out_python.txt` in the `output` directory.

    ```bash
    cat output/out_python.txt
    ```

## C++ code

Find the C++ code in GitHub [here](https://github.com/niklai99/ScientificComputing4Physics/blob/main/task1/test/scalar_prod_cpp.cpp).

```cpp linenums="1" title="scalar_prod_cpp.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const std::string OUTPUT_DIR = "output";
const std::string OUTPUT_FILE = OUTPUT_DIR + "/output_cpp.txt";

int main() {
    // Define the input values
    double a = 3;
    std::vector<double> x(20, 1.0);
    std::vector<double> y(20, 4.0);

    // Calculate z = a*x + y
    std::vector<double> z(20);
    for (int i = 0; i < 20; i++) {
        z[i] = a * x[i] + y[i];
    }

    // Write the result to a text file
    std::ofstream ofs(OUTPUT_FILE);
    if (!ofs) {
        std::cerr << "Error: Unable to open the output file\n";
        return 1;
    }
    for (double val : z) {
        ofs << val << std::endl;
    }
    ofs.close();

    return 0;
}

```

### Compiling and running the C++ program

1. Compile the C++ code:

    ```bash
    g++ -o scalar_prod_cpp scalar_prod_cpp.cpp
    ```

2. Run the executable:

    ```bash
    ./scalar_prod_cpp
    ```

3. Check the output file `output_cpp.txt` in the `output` directory:

    ```bash
    cat output/output_cpp.txt
    ```


!!! tip 
    If you want to store these files (both Python and C++ scripts) on your host machine permanently, place them in the shared directory you mounted (e.g., `/workspace`) so they persist after the container stops.