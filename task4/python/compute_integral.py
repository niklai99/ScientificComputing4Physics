#!/usr/bin/env python3
import numpy as np
from scipy.interpolate import interp1d

# Try to import Simpson’s rule function.
try:
    from scipy.integrate import simps
except ImportError:
    from scipy.integrate import simpson
    simps = simpson

# Try to import romberg; if not available, provide a fallback.
try:
    from scipy.integrate import romberg
except ImportError:
    def romberg(f, a, b, tol=1e-12, rtol=1e-12, divmax=20):
        # Fallback Romberg integration using the trapezoidal rule and Richardson extrapolation.
        R = []  # Romberg table (list of lists)
        R.append([(b - a) * (f(a) + f(b)) / 2.0])
        for i in range(1, divmax):
            n_intervals = 2**i
            h = (b - a) / n_intervals
            sum_ = 0.0
            for k in range(1, n_intervals, 2):
                sum_ += f(a + k * h)
            R0 = 0.5 * R[i-1][0] + h * sum_
            row = [R0]
            for j in range(1, i+1):
                factor = 4**j
                extrap = row[j-1] + (row[j-1] - R[i-1][j-1]) / (factor - 1.0)
                row.append(extrap)
            R.append(row)
            if i > 0 and abs(R[i][i] - R[i-1][i-1]) < tol:
                return R[i][i]
        return R[-1][-1]

def main():
    # Load data from the output file (assumed location: ./data/output.txt)
    data = np.loadtxt('./data/output.txt')
    x = data[:, 0]
    fx = data[:, 1]

    # Integration bounds: we want to integrate from 0 to π/2.
    a = 0.0
    b = np.pi / 2.0

    # Since the file is generated from 0 to π, extract the subset corresponding to [0, π/2].
    indices = np.where(x <= b)[0]
    x_subset = x[indices]
    fx_subset = fx[indices]

    # Compute the integral using the trapezoidal rule.
    integral_trapz = np.trapezoid(fx_subset, x_subset)

    # Compute the integral using Simpson's rule.
    integral_simpson = simps(fx_subset, x_subset)

    # Create an interpolation function over the subset for Romberg integration.
    f_interp = interp1d(x_subset, fx_subset, kind='cubic', fill_value="extrapolate")
    integral_romberg = romberg(f_interp, a, b, tol=1e-12, rtol=1e-12, divmax=20)

    # Print the results with 16-digit precision.
    print("Trapezoidal Integral: {:.16f}".format(integral_trapz))
    print("Simpson Integral:     {:.16f}".format(integral_simpson))
    print("Romberg Integral:     {:.16f}".format(integral_romberg))
    
    # Load the cpp output integrals called ./data/integral<Method>_N<sampling>_precision<precision>.txt
    cpp_integral_trapz = np.loadtxt('./data/integralTrapz_N1000000_precision16.txt')
    cpp_integral_simpson = np.loadtxt('./data/integralSimpson_N1000000_precision16.txt')
    cpp_integral_romberg = np.loadtxt('./data/integralRomberg_N1000000_precision16.txt')
    
    # compute the absolute difference between the python and cpp results
    abs_diff_trapz = abs(integral_trapz - cpp_integral_trapz)
    abs_diff_simpson = abs(integral_simpson - cpp_integral_simpson)
    abs_diff_romberg = abs(integral_romberg - cpp_integral_romberg)
    # Print the absolute differences
    print("Absolute Difference (Trapz): {:.16f}".format(abs_diff_trapz))
    print("Absolute Difference (Simpson): {:.16f}".format(abs_diff_simpson))
    print("Absolute Difference (Romberg): {:.16f}".format(abs_diff_romberg))

if __name__ == '__main__':
    main()
