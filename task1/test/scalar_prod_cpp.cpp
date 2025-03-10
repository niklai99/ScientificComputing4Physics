// Given the followinginput:
// a = 3-> a real-valued scalarnumber
// x= (1,1,1,1,1,..., 1)-> a real-valued vector of dimension N = 20
// y= (4,4,4,4,4,..., 4)-> a real-valued vector of dimension N = 20
// write a computer programthat calculates the following scalar product: 
// z = a*x + y
// and printits value on a text file

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Global variables
const std::string output_dir = "output";
const std::string output_dir_command = "mkdir -p " + output_dir;
const std::string output_file = output_dir + "/output_cpp.txt";

int main() {
    // Define the input values
    double a = 3;
    std::vector<double> x(20, 1);
    std::vector<double> y(20, 4);

    // Calculate the scalar product
    std::vector<double> z(20);
    for (int i = 0; i < 20; i++) {
        z[i] = a * x[i] + y[i];
    }

    // Write the result to a text file
    system(output_dir_command.c_str()); // check if the output directory exists
    std::ofstream output_file_stream(output_file);
    if (output_file_stream.is_open()) {
        for (int i = 0; i < 20; i++) {
            output_file_stream << z[i] << std::endl;
        }
        output_file_stream.close();
    } else {
        std::cerr << "Error: Unable to open the output file" << std::endl;
        return 1;
    }

    return 0;
}