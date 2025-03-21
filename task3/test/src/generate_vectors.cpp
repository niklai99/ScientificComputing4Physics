#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>

// Helper function: Extract the directory part from a path (everything before the last '/')
std::string extract_directory(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) return ".";
    return path.substr(0, pos);
}

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

    // Build filenames with underscores inserted automatically.
    std::ostringstream filename_x, filename_y;
    filename_x << prefix << "_N" << N << "_x.dat";
    filename_y << prefix << "_N" << N << "_y.dat";

    // Save vectors to files.
    std::ofstream file_x(filename_x.str());
    std::ofstream file_y(filename_y.str());
    if (!file_x || !file_y) {
        std::cerr << "Error: Unable to open output files." << std::endl;
        return 1;
    }
    for (size_t i = 0; i < N; ++i) {
        file_x << x[i] << "\n";
        file_y << y[i] << "\n";
    }
    std::cout << "Files generated: " << filename_x.str() << ", " << filename_y.str() << std::endl;
    return 0;
}
