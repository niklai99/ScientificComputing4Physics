# Code overview

This section explains how the C++ tool computeIntegral performs sampling and integration, and how the Python script compute_integral.py validates those results.

## Sampling and integration in C++

You run the C++ executable by supplying three options: the number of points (`N`), the lower limit (`x_inf`), and the upper limit (`x_sup`):

```bash
run computeIntegral 1000  0.0 3.14159
```

where `1000` is the number of sampling points, `0.0` is the lower limit, and `3.14159` is the upper limit.

Alternatively, you can use the `config/config.yml` file to specify these parameters.

```yaml
N: 1000
x_inf: 0.0
x_sup: 1.0
```

Internally, `computeIntegral.cpp` begins by parsing these arguments using:

```cpp
InputParameters params = readInputs(argc, argv);
```

Here, `readInputs()` constructs an `InputParameters` struct:

```cpp
struct InputParameters {
    int N;
    double x_inf;
    double x_sup;
};
```

Next, the program writes the sample points to `data/data_<N>.txt` using a loop:

```cpp
std::ofstream out = openOutputFile("data/data_" + std::to_string(params.N) + ".txt");

double dx = (params.x_sup - params.x_inf) / (params.N - 1);
for (int i = 0; i < params.N; ++i) {
    double x = params.x_inf + i * dx;
    double y = CosExpFunction()(x);
    out << std::fixed << std::setprecision(16)
        << x << " " << y << "\n";
}
out.close();
```

After sampling, these data feed into three integration routines. An Integrator object is created and its methods are invoked as follows:

```cpp
Integrator integrator;

double I_trapz  = integrator.integrateTrapz(params.x_inf, params.x_sup, params.N);
double I_simp   = integrator.integrateSimpson(params.x_inf, params.x_sup, params.N);
double I_romb   = integrator.integrateRomberg(params.x_inf, params.x_sup, 4, 1e-12);
```

These calls implement the Trapezoidal rule, Simpson’s rule, and Romberg integration respectively (see detailed pages for algorithmic specifics). Each computed value is then written to its own file:

```cpp
saveIntegralResult(DATA_DIR, "Trapz", params.N, OUTPUT_PRECISION, I_trapz);
saveIntegralResult(DATA_DIR, "Simpson", params.N, OUTPUT_PRECISION, I_simp);
saveIntegralResult(DATA_DIR, "Romberg", params.N, OUTPUT_PRECISION, I_romb);
```

## Validation with Python

To compare these results, you execute the Python script:

```bash
run compute_integral --N <N> --precision <precision> 
```

Within `compute_integral.py`, the script loads the sampled data and reads the C++ results from the output files. 

The script then recomputes each integral in Python:

```python
integral_trapz = trapezoid(fx_subset, x_subset)
integral_simpson = simpson(fx_subset, x_subset)
integral_romberg = romberg(interp1d(x_subset, fx_subset, kind='cubic', fill_value="extrapolate"), A, B, tol=ROMBERG_TOL, rtol=ROMBERG_RTOL, divmax=ROMBERG_DIVMAX)
```

