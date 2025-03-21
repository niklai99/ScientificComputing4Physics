#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include <yaml-cpp/yaml.h>
#include "H5Cpp.h"  // HDF5 C++ API

#include "VectorSumInterface.hpp"
#include "VectorSumDefault.hpp"
#include "VectorSumGSL.hpp"
#include "VectorIO.hpp"  // I/O helper functions
#include "HelperFunctions.hpp" 


// Display the configuration settings. This function is defined at the end of this file.
void displayConfig(const std::string configPath, const YAML::Node &config);


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }
    
    std::string configPath = argv[1];
    YAML::Node config;
    try {
        config = YAML::LoadFile(configPath);
    } catch (const YAML::BadFile &e) {
        std::cerr << "Error: Unable to open " << configPath << std::endl;
        return 1;
    }
    
    displayConfig(configPath, config);

    // Read input configuration.
    std::string file_x = config["input"]["vector_x"].as<std::string>();
    std::string file_y = config["input"]["vector_y"].as<std::string>();
    size_t N = config["input"]["N"].as<size_t>();
    double a = config["input"]["a"].as<double>();
    
    // Read output configuration.
    std::string output_path = config["output"]["path"].as<std::string>();
    std::string output_prefix = config["output"]["prefix"].as<std::string>();
    std::string out_format = config["output"]["format"].as<std::string>();  // Use this for output file extension.
    std::string implementation = config["implementation"].as<std::string>(); // "default" or "gsl"

    // Ensure output path ends with a slash. Otherwise, insert one.
    output_path = ensure_trailing_slash(output_path);
    
    // Validate input file extensions.
    std::vector<std::string> allowed = {"txt", "dat", "h5"};
    if (!validate_extensions(file_x, allowed) || !validate_extensions(file_y, allowed)) {
        std::cerr << "Error: Unsupported file extension." << std::endl;
        return 1;
    }
    
    // Read input vectors using our common I/O functions.
    std::vector<double> x, y;
    if (!read_vector(file_x, N, x) || !read_vector(file_y, N, y)) {
        std::cerr << "Error: Failed to read input vectors." << std::endl;
        return 1;
    }
    
    // Instantiate the appropriate vector sum implementation.
    std::unique_ptr<VectorSumInterface> vs;
    if (implementation == "default") {
        vs.reset(new VectorSumDefault());
    } else if (implementation == "gsl") {
        vs.reset(new VectorSumGSL());
    } else {
        std::cerr << "Unsupported implementation: " << implementation << std::endl;
        return 1;
    }
    
    // Compute vector sum.
    std::vector<double> d;
    vs->compute_sum(x, y, a, d);
    
    // Create output directory if it doesn't exist.
    std::string cmd = "mkdir -p " + output_path;
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Error: Could not create directory " << output_path << std::endl;
        return 1;
    }
    
    // Build the output filename as: <output_path><prefix>_N<value>_d.<ext>
    std::string out_filename = output_path + output_prefix + "_N" + std::to_string(N) + "_d." + out_format;
    
    if (!write_vector(out_filename, d)) {
        std::cerr << "Error: Failed to write output vector." << std::endl;
        return 1;
    }
    
    std::cout << "Vector sum computed and saved to " << out_filename << std::endl;
    return 0;
}



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