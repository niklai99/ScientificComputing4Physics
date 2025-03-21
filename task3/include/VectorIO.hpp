#ifndef VECTOR_IO_HPP
#define VECTOR_IO_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "H5Cpp.h"      // Include the HDF5 C++ API for HDF5 file I/O.
#include "HelperFunctions.hpp"  // Includes helper functions like get_extension(), extract_directory(), etc.

/**
 * @brief Reads a vector of doubles from a file.
 *
 * The file format is determined by the file extension:
 * - For "txt" or "dat": plain text I/O is used.
 * - For "h5": the HDF5 C++ API is used to read the vector from a dataset.
 *
 * We rely on the helper function get_extension() to determine the file type.
 *
 * @param filename The input filename.
 * @param N The expected number of elements.
 * @param vec Output vector to store the data.
 * @return true if successful, false otherwise.
 */
inline bool read_vector(const std::string &filename, size_t N, std::vector<double> &vec) {
    // Determine the file extension (without the dot) using our helper function.
    std::string ext = get_extension(filename);

    // If the file is plain text (either "txt" or "dat")...
    if (ext == "txt" || ext == "dat") {
        // Open the file for reading.
        std::ifstream in_file(filename);
        if (!in_file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }
        // Resize the output vector to hold N elements.
        vec.resize(N);
        // Read each element from the file.
        for (size_t i = 0; i < N; ++i) {
            in_file >> vec[i];
            // If reading fails at any point, report an error and return false.
            if (in_file.fail()) {
                std::cerr << "Error: Failed reading element " << i << " from " << filename << std::endl;
                return false;
            }
        }
        in_file.close();  // Close the file.
        return true;      // Reading was successful.
    } 
    // Else, if the file is in HDF5 format...
    else if (ext == "h5") {
        try {
            // Decide the dataset name based on the filename.
            // Convention: if the filename contains "_x.", assume dataset name "x", otherwise "y".
            std::string ds_name = (filename.find("_x.") != std::string::npos) ? "x" : "y";

            // Open the HDF5 file in read-only mode.
            H5::H5File file(filename, H5F_ACC_RDONLY);
            
            // Open the dataset with the chosen name.
            H5::DataSet dataset = file.openDataSet(ds_name);
            
            // Get the dataspace of the dataset to determine its dimensions.
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims, nullptr);
            
            // Check if the dataset's size matches the expected number of elements N.
            if (dims[0] != N) {
                std::cerr << "Error: Dimension mismatch in file " << filename << std::endl;
                return false;
            }
            
            // Resize the output vector to hold N elements.
            vec.resize(N);
            
            // Read the data from the dataset into the vector.
            dataset.read(vec.data(), H5::PredType::NATIVE_DOUBLE);
            
            file.close();  // Close the HDF5 file.
            return true;   // Reading was successful.
        } catch (H5::Exception &error) {
            // If an HDF5 exception occurs, print the error stack and return false.
            error.printErrorStack();
            return false;
        }
    } 
    // If the extension is not supported...
    else {
        std::cerr << "Error: Unsupported file extension in " << filename << std::endl;
        return false;
    }
}

/**
 * @brief Writes a vector of doubles to a file.
 *
 * The file format is determined by the file extension:
 * - For "txt" or "dat": plain text I/O is used.
 * - For "h5": the HDF5 C++ API is used to write the vector into a dataset.
 *
 * We rely on the helper function get_extension() to determine the file type.
 *
 * @param filename The output filename.
 * @param vec The vector to write.
 * @return true if successful, false otherwise.
 */
inline bool write_vector(const std::string &filename, const std::vector<double> &vec) {
    // Determine the number of elements in the vector.
    size_t N = vec.size();
    
    // Determine the file extension to choose the appropriate I/O method.
    std::string ext = get_extension(filename);
    
    // If writing to a plain text file ("txt" or "dat")...
    if (ext == "txt" || ext == "dat") {
        // Open the file for writing.
        std::ofstream out_file(filename);
        if (!out_file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
            return false;
        }
        // Write each element of the vector to the file, one per line.
        for (size_t i = 0; i < N; ++i) {
            out_file << vec[i] << "\n";
        }
        out_file.close();  // Close the file.
        return true;       // Writing was successful.
    } 
    // Else, if writing to an HDF5 file...
    else if (ext == "h5") {
        try {
            // Open the HDF5 file in truncate mode (creating a new file or overwriting an existing one).
            H5::H5File file(filename, H5F_ACC_TRUNC);
            
            // Set up the dataspace for a 1D dataset of size N.
            hsize_t dims[1] = { N };
            H5::DataSpace dataspace(1, dims);
            
            // Create the dataset with the name "d".
            H5::DataSet dataset = file.createDataSet("d", H5::PredType::NATIVE_DOUBLE, dataspace);
            
            // Write the vector data to the dataset.
            dataset.write(vec.data(), H5::PredType::NATIVE_DOUBLE);
            
            file.close();  // Close the HDF5 file.
            return true;   // Writing was successful.
        } catch (H5::Exception &error) {
            // If an HDF5 exception occurs, print the error stack and return false.
            error.printErrorStack();
            return false;
        }
    } 
    // If the file extension is not supported...
    else {
        std::cerr << "Error: Unsupported file extension in " << filename << std::endl;
        return false;
    }
}

#endif // VECTOR_IO_HPP
