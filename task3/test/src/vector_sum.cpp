#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <yaml-cpp/yaml.h>
#include "vector_sum.hpp"

// Helper: Extracts the extension (without the dot) from a filename.
std::string get_extension(const std::string &filename) {
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos)
        return "";
    return filename.substr(pos + 1);
}

// Helper: Extracts the directory part from a path (everything before the last '/').
std::string extract_directory(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        return ".";
    return path.substr(0, pos);
}

// Helper: Ensures that a path ends with a '/'
std::string ensure_trailing_slash(const std::string &path) {
    if (path.empty() || path.back() == '/')
        return path;
    return path + "/";
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
    std::cout << "  Implementation: " << config["implementation"].as<std::string>() << std::endl;
}

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
    
    // Display the configuration values.
    displayConfig(configPath, config);

    // Read input configuration values.
    std::string file_x = config["input"]["vector_x"].as<std::string>();
    std::string file_y = config["input"]["vector_y"].as<std::string>();
    size_t N = config["input"]["N"].as<size_t>();
    double a = config["input"]["a"].as<double>();
    
    // Read output configuration.
    std::string output_path = config["output"]["path"].as<std::string>();
    std::string output_prefix = config["output"]["prefix"].as<std::string>();
    std::string implementation = config["implementation"].as<std::string>(); // "default" or "gsl"

    // Ensure output_path ends with a slash.
    output_path = ensure_trailing_slash(output_path);
    
    // Determine file format based on the extension of file_x (and file_y).
    std::string ext_x = get_extension(file_x);
    std::string ext_y = get_extension(file_y);
    
    if (ext_x != ext_y) {
        std::cerr << "Error: Input files must have the same extension." << std::endl;
        return 1;
    }
    
    // Supported extensions: "txt", "dat", "hdf5"
    if (ext_x != "txt" && ext_x != "dat" && ext_x != "hdf5") {
        std::cerr << "Error: Unsupported file extension: " << ext_x << std::endl;
        return 1;
    }
    
    // Read vectors from input files.
    std::vector<double> x(N), y(N);
    if (ext_x == "txt" || ext_x == "dat") {
        std::ifstream in_x(file_x);
        std::ifstream in_y(file_y);
        if (!in_x.is_open() || !in_y.is_open()) {
            std::cerr << "Error: Could not open input files." << std::endl;
            return 1;
        }
        for (size_t i = 0; i < N; ++i) {
            in_x >> x[i];
            in_y >> y[i];
        }
        in_x.close();
        in_y.close();
    } else if (ext_x == "hdf5") {
        // HDF5 reading: Placeholder - implement using HDF5 C++ API.
        std::cerr << "Error: HDF5 reading not implemented yet." << std::endl;
        return 1;
    }
    
    // Compute vector sum using the specified implementation.
    std::vector<double> d;
    if (implementation == "default") {
        default_vector_sum(x, y, a, d);
    } else {
        std::cerr << "Unsupported implementation: " << implementation << std::endl;
        return 1;
    }
    
    // Create output directory if it does not exist (use output_path).
    std::string cmd = "mkdir -p " + output_path;
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Error: Could not create directory " << output_path << std::endl;
        return 1;
    }
    
    // Build the output filename as: <output_path><prefix>_N<value>_d.<ext>
    std::string out_filename = output_path + output_prefix + "_N" + std::to_string(N) + "_d." + ext_x;
    
    // Write the output vector to file.
    if (ext_x == "txt" || ext_x == "dat") {
        std::ofstream out_file(out_filename);
        if (!out_file.is_open()) {
            std::cerr << "Error: Could not open output file: " << out_filename << std::endl;
            return 1;
        }
        for (size_t i = 0; i < d.size(); ++i) {
            out_file << d[i] << "\n";
        }
        out_file.close();
    } else if (ext_x == "hdf5") {
        // HDF5 writing: Placeholder - implement using HDF5 C++ API.
        std::cerr << "Error: HDF5 writing not implemented yet." << std::endl;
        return 1;
    }
    
    std::cout << "Vector sum computed and saved to " << out_filename << std::endl;
    return 0;
}
