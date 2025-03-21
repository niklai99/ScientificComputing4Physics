#ifndef VECTOR_IO_HPP
#define VECTOR_IO_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "H5Cpp.h"      // HDF5 C++ API
#include "HelperFunctions.hpp"  // Contains get_extension(), extract_directory(), etc.

/**
 * @brief Reads a vector of doubles from a file.
 *
 * The file format is determined by the file extension: for "txt" or "dat", plain text I/O is used;
 * for "h5", the HDF5 C++ API is used.
 *
 * @param filename The input filename.
 * @param N The expected number of elements.
 * @param vec Output vector to store the data.
 * @return true if successful, false otherwise.
 */
inline bool read_vector(const std::string &filename, size_t N, std::vector<double> &vec) {
    std::string ext = get_extension(filename);
    if (ext == "txt" || ext == "dat") {
        std::ifstream in_file(filename);
        if (!in_file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }
        vec.resize(N);
        for (size_t i = 0; i < N; ++i) {
            in_file >> vec[i];
            if (in_file.fail()) {
                std::cerr << "Error: Failed reading element " << i << " from " << filename << std::endl;
                return false;
            }
        }
        in_file.close();
        return true;
    } else if (ext == "h5") {
        try {
            // Decide dataset name based on filename pattern: if "_x." is in the filename, assume "x", otherwise "y"
            std::string ds_name = (filename.find("_x.") != std::string::npos) ? "x" : "y";
            H5::H5File file(filename, H5F_ACC_RDONLY);
            H5::DataSet dataset = file.openDataSet(ds_name);
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims, nullptr);
            if (dims[0] != N) {
                std::cerr << "Error: Dimension mismatch in file " << filename << std::endl;
                return false;
            }
            vec.resize(N);
            dataset.read(vec.data(), H5::PredType::NATIVE_DOUBLE);
            file.close();
            return true;
        } catch (H5::Exception &error) {
            error.printErrorStack();
            return false;
        }
    } else {
        std::cerr << "Error: Unsupported file extension in " << filename << std::endl;
        return false;
    }
}

/**
 * @brief Writes a vector of doubles to a file.
 *
 * The file format is determined by the file extension: for "txt" or "dat", plain text I/O is used;
 * for "h5", the HDF5 C++ API is used.
 *
 * @param filename The output filename.
 * @param vec The vector to write.
 * @return true if successful, false otherwise.
 */
inline bool write_vector(const std::string &filename, const std::vector<double> &vec) {
    size_t N = vec.size();
    std::string ext = get_extension(filename);
    if (ext == "txt" || ext == "dat") {
        std::ofstream out_file(filename);
        if (!out_file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
            return false;
        }
        for (size_t i = 0; i < N; ++i) {
            out_file << vec[i] << "\n";
        }
        out_file.close();
        return true;
    } else if (ext == "h5") {
        try {
            // For output, we use the dataset name "d".
            H5::H5File file(filename, H5F_ACC_TRUNC);
            hsize_t dims[1] = { N };
            H5::DataSpace dataspace(1, dims);
            H5::DataSet dataset = file.createDataSet("d", H5::PredType::NATIVE_DOUBLE, dataspace);
            dataset.write(vec.data(), H5::PredType::NATIVE_DOUBLE);
            file.close();
            return true;
        } catch (H5::Exception &error) {
            error.printErrorStack();
            return false;
        }
    } else {
        std::cerr << "Error: Unsupported file extension in " << filename << std::endl;
        return false;
    }
}

#endif // VECTOR_IO_HPP
