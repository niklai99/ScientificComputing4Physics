# Sampling and Data I/O

This page describes how the C++ executable `computeIntegral` reads input parameters, generates sampling points, evaluates the function, and writes data to disk in a text file.

## Input parameter handling

At program start, command‑line arguments are parsed by `readInputs`, returning an `InputParameters` struct:

```cpp
InputParameters params;
params = readInputs(argc, argv);
```

- `params.N` specifies the number of sampling points.
- `params.x_inf`, `params.x_sup`: lower and upper bounds for the integration.

Errors during parsing throw an exception, caught and reported to `std::cerr`.

## Ensuring output directory 

Before writing any data, the directory `./data` is created (if missing) via:

```cpp
createDataDirectory("./data");
```

This helper wraps `mkdir -p` and throws on failure.

## Sampling 

The function under study is

```cpp
class CosExpFunction : public Function {
  double operator()(double x) const override {
    return std::cos(x) * std::exp(x);
  }
};
```

Once an instance is created:

```cpp
std::ofstream outFile = openOutputFile(DATA_FILE);
CosExpFunction f;
double dx = (params.x_sup - params.x_inf) / (params.N - 1);
for (int i = 0; i < params.N; ++i) {
    double x = params.x_inf + i * dx;
    outFile << std::fixed << std::setprecision(OUTPUT_PRECISION)
            << x << " " << f(x) << "\n";
}
outFile.close();
```

After completion, the program prints the number of points and file location.