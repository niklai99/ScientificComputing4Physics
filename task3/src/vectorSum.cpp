#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include <yaml-cpp/yaml.h>
#include "H5Cpp.h"

// Include the polymorphic vector sum interface and its two implementations.
#include "VectorSumInterface.hpp"
#include "VectorSumDefault.hpp"
#include "VectorSumGSL.hpp"

// Include I/O helper functions (for reading and writing vectors in text or HDF5 format)
// and common helper functions for path manipulations.
#include "VectorIO.hpp"
#include "HelperFunctions.hpp"

/**
 * @brief Displays the configuration settings.
 *
 * This function prints the key parameters from the configuration file,
 * making it easier for the user and developers to verify the loaded settings.
 *
 * @param configPath The path to the configuration file.
 * @param config The YAML node representing the configuration.
 */
void displayConfig(const std::string configPath, const YAML::Node &config) {
    std::cout << "Configuration: " << configPath << std::endl;
    std::cout << "  Input:" << std::endl;
    std::cout << "    vector_x: " << config["input"]["vector_x"].as<std::string>() << std::endl;
    std::cout << "    vector_y: " << config["input"]["vector_y"].as<std::string>() << std::endl;
    std::cout << "    N: " << config["input"]["N"].as<size_t>() << std::endl;
    std::cout << "    a: " << config["input"]["a"].as<double>() << std::endl;
    std::cout << "  Output:" << std::endl;
    std::cout << "    path: " << config["output"]["path"].as<std::string>() << std::endl;
    std::cout << "    prefix: " << config["output"]["prefix"].as<std::string>() << std::endl;
    std::cout << "    format: " << config["output"]["format"].as<std::string>() << std::endl;
    std::cout << "  Implementation: " << config["implementation"].as<std::string>() << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
    // Check that the configuration file path is provided as a command-line argument.
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }
    
    // Load the configuration file using YAML-CPP.
    std::string configPath = argv[1];
    YAML::Node config;
    try {
        config = YAML::LoadFile(configPath);
    } catch (const YAML::BadFile &e) {
        std::cerr << "Error: Unable to open " << configPath << std::endl;
        return 1;
    }
    
    // Display the loaded configuration for verification.
    displayConfig(configPath, config);

    // Read input configuration parameters.
    std::string file_x = config["input"]["vector_x"].as<std::string>(); // File for vector x.
    std::string file_y = config["input"]["vector_y"].as<std::string>(); // File for vector y.
    size_t N = config["input"]["N"].as<size_t>();                     // Expected number of elements.
    double a = config["input"]["a"].as<double>();                      // Scalar multiplier.

    // Read output configuration parameters.
    std::string output_path = config["output"]["path"].as<std::string>();    // Directory for output.
    std::string output_prefix = config["output"]["prefix"].as<std::string>(); // Output file prefix.
    std::string out_format = config["output"]["format"].as<std::string>();    // Output file extension/format.
    std::string implementation = config["implementation"].as<std::string>();  // Which vector sum implementation to use.

    // Ensure the output path ends with a slash for proper concatenation.
    output_path = ensure_trailing_slash(output_path);
    
    // Validate the input file extensions independently using our helper functions.
    // (Note: We allow them to be different if desired, but you could enforce sameness if needed.)
    if (!validate_extensions(file_x, std::vector<std::string>{"txt", "dat", "h5"}) ||
        !validate_extensions(file_y, std::vector<std::string>{"txt", "dat", "h5"})) {
        std::cerr << "Error: One or more input files have unsupported extensions." << std::endl;
        return 1;
    }
    
    // Read the input vectors using our common I/O functions defined in VectorIO.hpp.
    std::vector<double> x, y;
    if (!read_vector(file_x, N, x)) {
        std::cerr << "Error: Failed to read input vector from " << file_x << std::endl;
        return 1;
    }
    if (!read_vector(file_y, N, y)) {
        std::cerr << "Error: Failed to read input vector from " << file_y << std::endl;
        return 1;
    }
    
    // Instantiate the appropriate vector sum implementation based on the configuration.
    // The choice is made at runtime, but both implementations share the same interface.
    std::unique_ptr<VectorSumInterface> vs;
    if (implementation == "default") {
        vs.reset(new VectorSumDefault());
    } else if (implementation == "gsl") {
        vs.reset(new VectorSumGSL());
    } else {
        std::cerr << "Unsupported implementation: " << implementation << std::endl;
        return 1;
    }
    
    // Compute the vector sum using the chosen implementation.
    std::vector<double> d;
    vs->compute_sum(x, y, a, d);
    
    // Create the output directory if it does not exist.
    std::string cmd = "mkdir -p " + output_path;
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Error: Could not create directory " << output_path << std::endl;
        return 1;
    }
    
    // Build the output filename using the output configuration.
    // The naming convention is: <output_path><prefix>_N<value>_d.<format>
    std::string out_filename = output_path + output_prefix + "_N" + std::to_string(N) + "_d." + out_format;
    
    // Write the computed vector sum to the output file using our helper function.
    if (!write_vector(out_filename, d)) {
        std::cerr << "Error: Failed to write output vector to " << out_filename << std::endl;
        return 1;
    }
    
    // Inform the user that the vector sum has been successfully computed and saved.
    std::cout << "Vector sum computed and saved to " << out_filename << std::endl;
    return 0;
}