#include <iostream>
#include <fstream>
#include <iomanip>
#include "HelperFunctions.hpp"
#include "Function.hpp"
#include "Integrator.hpp"

const std::string DATA_DIR = "./data";
const std::string OUTPUT_FILE = DATA_DIR + "/output.txt";

const double ANALYTIC_SOLUTION = (std::exp(M_PI / 2.0) - 1.0) / 2.0;

int main(int argc, char* argv[]) {
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

    // Open the output file.
    std::ofstream outFile;
    try {
        outFile = openOutputFile(OUTPUT_FILE);
    } catch (const std::exception& e) {
        std::cerr << "File Error: " << e.what() << std::endl;
        return 1;
    }

    // Create an instance of the function f(x)=cos(x)*exp(x)
    CosExpFunction f;

    // Write the sampled points into the file.
    double dx = (params.x_sup - params.x_inf) / (params.N - 1);
    for (int i = 0; i < params.N; ++i) {
        double x = params.x_inf + i * dx;
        outFile << std::fixed << std::setprecision(16) << x << " " << f(x) << "\n";
    }
    outFile.close();

    // Create an Integrator instance and compute the integral from 0 to 2π.
    Integrator integrator(f);

    // Integrate the function from 0 to π/2 with N sampling points.
    double integralTrapz;
    double integralSimpson;
    double integralRomberg;
    try {
        integralTrapz = integrator.integrateTrapz(0.0, M_PI / 2.0, params.N);
        integralSimpson = integrator.integrateSimpson(0.0, M_PI / 2.0, params.N);
        integralRomberg = integrator.integrateRomberg(0.0, M_PI / 2.0, 10, 1e-12);
    } catch (const std::exception& e) {
        std::cerr << "Integration Error: " << e.what() << std::endl;
        return 1;
    }

    // Print the computed integral with 16-digit precision.
    std::cout << "\nTrapezoidal Rule:" << std::endl;
    printIntegral(integralTrapz, 16, params.N, ANALYTIC_SOLUTION);
    std::cout << "\nSimpson's Rule:" << std::endl;
    printIntegral(integralSimpson, 16, params.N, ANALYTIC_SOLUTION);
    std::cout << "\nRomberg Integration:" << std::endl;
    printIntegral(integralRomberg, 16, params.N, ANALYTIC_SOLUTION);

    // Save integral results in a file called "integral<Method>_N<N>_precision<Precision>.txt"
    std::ofstream integralFile;
    try {
        integralFile.open(DATA_DIR + "/integralTrapz_N" + std::to_string(params.N) + "_precision16.txt");
        integralFile << std::fixed << std::setprecision(16) << integralTrapz;
        integralFile.close();

        integralFile.open(DATA_DIR + "/integralSimpson_N" + std::to_string(params.N) + "_precision16.txt");
        integralFile << std::fixed << std::setprecision(16) << integralSimpson;
        integralFile.close();

        integralFile.open(DATA_DIR + "/integralRomberg_N" + std::to_string(params.N) + "_precision16.txt");
        integralFile << std::fixed << std::setprecision(16) << integralRomberg;
        integralFile.close();
    } catch (const std::exception& e) {
        std::cerr << "File Error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "\nResults saved to " << DATA_DIR << " directory." << std::endl;

    return 0;
}
