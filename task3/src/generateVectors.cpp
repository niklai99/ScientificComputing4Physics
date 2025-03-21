#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include "H5Cpp.h"  // HDF5 C++ API

#include "HelperFunctions.hpp"  // Contains extract_directory(), ensure_trailing_slash(), get_extension(), etc.
#include "VectorIO.hpp"         // Contains read_vector() and write_vector()

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <filename_prefix>" << std::endl;
        return 1;
    }
    size_t N = std::atoi(argv[1]);
    std::string prefix = argv[2];

    // Remove trailing underscore if present.
    if (!prefix.empty() && prefix.back() == '_') {
        prefix.pop_back();
    }

    // Create the output directory if it does not exist.
    std::string outDir = extract_directory(prefix);
    std::string cmd = "mkdir -p " + outDir;
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Error: Could not create directory " << outDir << std::endl;
        return 1;
    }

    // Create vectors with constant values.
    std::vector<double> x(N, 0.1);
    std::vector<double> y(N, 7.1);

    // Determine if the prefix already has an extension.
    std::string ext = get_extension(prefix);
    std::string base;
    if (!ext.empty()) {
        // Remove the extension (dot + extension length) from the prefix.
        base = prefix.substr(0, prefix.size() - (ext.size() + 1));
    } else {
        base = prefix;
        ext = "dat";
    }

    // Build filenames with underscores inserted automatically.
    std::ostringstream filename_x, filename_y;
    filename_x << base << "_N" << N << "_x." << ext;
    filename_y << base << "_N" << N << "_y." << ext;

    std::string file_x_str = filename_x.str();
    std::string file_y_str = filename_y.str();

    // Write the vectors to file using our I/O helper.
    if (!write_vector(file_x_str, x) || !write_vector(file_y_str, y)) {
        std::cerr << "Error: Failed to write output vectors." << std::endl;
        return 1;
    }

    std::cout << "Files generated: " << file_x_str << ", " << file_y_str << std::endl;
    return 0;
}
