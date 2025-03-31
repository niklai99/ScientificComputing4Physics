#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <cstdlib>  // for system()
#include <yaml-cpp/yaml.h>

// Structure to hold input parameters.
struct InputParameters {
    int N;
    double x_inf;
    double x_sup;
};

// Read input parameters either from command-line arguments or a YAML file.
inline InputParameters readInputs(int argc, char* argv[]) {
    InputParameters params;
    if (argc == 4) {
        try {
            params.N     = std::stoi(argv[1]);
            params.x_inf = std::stod(argv[2]);
            params.x_sup = std::stod(argv[3]);
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid command-line arguments.");
        }
    } else if (argc == 2) {
        std::string configPath = argv[1];
        YAML::Node config;
        try {
            config = YAML::LoadFile(configPath);
        } catch (const YAML::BadFile& e) {
            throw std::runtime_error("Unable to open configuration file.");
        }
        try {
            params.N     = config["N"].as<int>();
            params.x_inf = config["x_inf"].as<double>();
            params.x_sup = config["x_sup"].as<double>();
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid configuration file format.");
        }
    } else {
        throw std::runtime_error(
            "Usage:\n"
            "  <program> <N> <x_inf> <x_sup>\n"
            "or\n"
            "  <program> <config.yml>"
        );
    }
    return params;
}

// Create the data directory using a system call.
// The default directory is "./data".
inline void createDataDirectory(const std::string& dirPath = "./data") {
    std::string cmd = "mkdir -p " + dirPath;
    if (system(cmd.c_str()) != 0) {
        throw std::runtime_error("Error: Could not create directory " + dirPath);
    }
}

// Open an output file stream; throws an error if the file cannot be opened.
inline std::ofstream openOutputFile(const std::string& filePath) {
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open output file: " + filePath);
    }
    return outFile;
}

// Print the computed integral with a given precision.
inline void printIntegral(double integral, int precision = 16, int n_samp = 0, double ANALYTIC_SOLUTION = 0.0) {
    // Compute the relative error epsrel = I / Itrue - 1
    double epsrel = (integral / ANALYTIC_SOLUTION) - 1.0;

    // Print the results
    std::cout << "Analytic Solution:\t" << std::fixed << std::setprecision(precision)
                << ANALYTIC_SOLUTION << std::endl;
    std::cout << "Computed Integral:\t" << std::fixed << std::setprecision(precision)
                << integral << std::endl;
    std::cout << "Relative Error:\t\t" << std::fixed << std::setprecision(precision) 
                << epsrel << std::endl;
    std::cout << "Number of Sampling Points: " << n_samp << std::endl;
}

#endif  // HELPER_FUNCTIONS_HPP
