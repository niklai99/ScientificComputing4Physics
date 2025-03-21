#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include "H5Cpp.h"

// Include helper functions for file and path manipulation (e.g., extract_directory(), ensure_trailing_slash(), get_extension()).
#include "HelperFunctions.hpp"
// Include the I/O helper functions for reading/writing vectors.
#include "VectorIO.hpp"

int main(int argc, char* argv[]) {
    // Check that the correct number of command-line arguments are provided.
    // Expected usage: generateVectors <N> <filename_prefix>
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <filename_prefix>" << std::endl;
        return 1;
    }

    // Convert the first argument (number of elements) from string to size_t.
    size_t N = std::atoi(argv[1]);
    // The second argument is the filename prefix (which may include a directory and optionally an extension).
    std::string prefix = argv[2];

    // Remove a trailing underscore if present in the prefix.
    // Why: This avoids an extra underscore in the final filename.
    if (!prefix.empty() && prefix.back() == '_') {
        prefix.pop_back();
    }

    // Determine the output directory from the prefix.
    // extract_directory() returns the directory portion of the given path.
    std::string outDir = extract_directory(prefix);
    // Build a shell command to create the directory if it does not already exist.
    std::string cmd = "mkdir -p " + outDir;
    // Execute the command. If it fails, report an error and exit.
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Error: Could not create directory " << outDir << std::endl;
        return 1;
    }

    // Create the input vectors with constant values.
    // x is filled with 0.1, and y is filled with 7.1.
    std::vector<double> x(N, 0.1);
    std::vector<double> y(N, 7.1);

    // Determine if the prefix already has a file extension.
    // get_extension() returns the part of the filename after the last '.'.
    std::string ext = get_extension(prefix);
    std::string base;
    if (!ext.empty()) {
        // If an extension is present, remove it from the prefix to get the base name.
        // This prevents the extension from being duplicated in the final filename.
        base = prefix.substr(0, prefix.size() - (ext.size() + 1));
    } else {
        // If no extension is found, use the entire prefix as the base and default the extension to "dat".
        base = prefix;
        ext = "dat";
    }

    // Build the final output filenames for the two vectors.
    // The convention is: <base>_N<value>_x.<ext> and <base>_N<value>_y.<ext>
    std::ostringstream filename_x, filename_y;
    filename_x << base << "_N" << N << "_x." << ext;
    filename_y << base << "_N" << N << "_y." << ext;
    std::string file_x_str = filename_x.str();
    std::string file_y_str = filename_y.str();

    // Write the vectors to file using the unified write_vector() helper function.
    // This function automatically selects the appropriate I/O method based on the file extension.
    if (!write_vector(file_x_str, x) || !write_vector(file_y_str, y)) {
        std::cerr << "Error: Failed to write output vectors." << std::endl;
        return 1;
    }

    // Inform the user that the files were successfully generated.
    std::cout << "Files generated: " << file_x_str << ", " << file_y_str << std::endl;
    return 0;
}
