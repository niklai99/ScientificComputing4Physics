// integrate_function.cpp
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "HelperFunctions.hpp"
#include "Function.hpp"
#include "Integrator.hpp"

// Constants for data output and integration bounds.
const std::string DATA_DIR = "./data";
const std::string DATA_NAME = "data";
const int OUTPUT_PRECISION = 16;

// Integration interval: [0, π/2]
const double INTEGRATION_A = 0.0;
const double INTEGRATION_B = M_PI / 2.0;

// Analytic solution for comparison.
const double ANALYTIC_SOLUTION = (std::exp(M_PI / 2.0) - 1.0) / 2.0;

int main(int argc, char* argv[]) {
    // Read input parameters.
    InputParameters params;
    try {
        params = readInputs(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Input Error: " << e.what() << std::endl;
        return 1;
    }

    // Ensure the output directory exists.
    try {
        createDataDirectory(DATA_DIR);
    } catch (const std::exception& e) {
        std::cerr << "Directory Error: " << e.what() << std::endl;
        return 1;
    }

    // Create the data output file path.
    std::string DATA_FILE = DATA_DIR + "/" + DATA_NAME + "_" + std::to_string(params.N) + ".txt";
    std::ofstream outFile;
    try {
        outFile = openOutputFile(DATA_FILE);
    } catch (const std::exception& e) {
        std::cerr << "File Error: " << e.what() << std::endl;
        return 1;
    }

    // Create an instance of the function f(x) = cos(x) * exp(x)
    CosExpFunction f;

    // Generate and write N uniformly spaced points over [params.x_inf, params.x_sup].
    double dx = (params.x_sup - params.x_inf) / (params.N - 1);
    for (int i = 0; i < params.N; ++i) {
        double x = params.x_inf + i * dx;
        outFile << std::fixed << std::setprecision(OUTPUT_PRECISION)
                << x << " " << f(x) << "\n";
    }
    outFile.close();

    std::cout << "Generated " << params.N << " points in the range ["
              << params.x_inf << ", " << params.x_sup << "]." << std::endl;
    std::cout << "Data saved to " << DATA_FILE << std::endl;

    // Create an Integrator instance.
    Integrator integrator(f);

    // Compute the actual number of points in the integration interval [INTEGRATION_A, INTEGRATION_B]
    // given dx from the generated data.
    const int N_AB = static_cast<int>(std::round((INTEGRATION_B - INTEGRATION_A) / dx)) + 1;

    double integralTrapz, integralSimpson, integralRomberg;
    try {
        integralTrapz   = integrator.integrateTrapz(INTEGRATION_A, INTEGRATION_B, N_AB);
        integralSimpson = integrator.integrateSimpson(INTEGRATION_A, INTEGRATION_B, N_AB);
        integralRomberg = integrator.integrateRomberg(INTEGRATION_A, INTEGRATION_B, 10, 1e-12);
    } catch (const std::exception& e) {
        std::cerr << "Integration Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Integration bounds: [" << INTEGRATION_A << ", " << INTEGRATION_B << "]" << std::endl;
    std::cout << "\nTrapezoidal Rule:" << std::endl;
    printIntegral(integralTrapz, OUTPUT_PRECISION, N_AB, ANALYTIC_SOLUTION);
    std::cout << "\nSimpson's Rule:" << std::endl;
    printIntegral(integralSimpson, OUTPUT_PRECISION, N_AB, ANALYTIC_SOLUTION);
    std::cout << "\nRomberg Integration:" << std::endl;
    printIntegral(integralRomberg, OUTPUT_PRECISION, N_AB, ANALYTIC_SOLUTION);

    // Save the integral results to files.
    try {
    
        saveIntegralResult(DATA_DIR, "Trapz", N_AB, OUTPUT_PRECISION, integralTrapz);
        saveIntegralResult(DATA_DIR, "Simpson", N_AB, OUTPUT_PRECISION, integralSimpson);
        saveIntegralResult(DATA_DIR, "Romberg", N_AB, OUTPUT_PRECISION, integralRomberg);
    } catch (const std::exception& e) {
        std::cerr << "File Error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "\nResults saved to " << DATA_DIR << " directory." << std::endl;

    // Output a CSV row (fields: Language, N_AB, Trapz, Simpson, Romberg, Analytic)
    std::cout << "cpp," << N_AB << ","
              << std::fixed << std::setprecision(OUTPUT_PRECISION)
              << integralTrapz << ","
              << integralSimpson << ","
              << integralRomberg << ","
              << ANALYTIC_SOLUTION << std::endl;

    return 0;
}
