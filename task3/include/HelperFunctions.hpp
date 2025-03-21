#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

/**
 * @brief Extracts the directory part from a path (everything before the last '/').
 */
inline std::string extract_directory(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        return ".";
    return path.substr(0, pos);
}

/**
 * @brief Ensures that a path ends with a '/'.
 */
inline std::string ensure_trailing_slash(const std::string &path) {
    if (path.empty() || path.back() == '/')
        return path;
    return path + "/";
}

/**
 * @brief Extracts the extension (without the dot) from a filename.
 */
inline std::string get_extension(const std::string &filename) {
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos)
        return "";
    return filename.substr(pos + 1);
}

/**
 * @brief Validates that two filenames have the same allowed extension.
 *
 * @param file1 First filename.
 * @param file2 Second filename.
 * @param allowed List of allowed extensions.
 * @param ext (Output) the extension if valid.
 * @return true if valid, false otherwise.
 */
inline bool validate_extensions(const std::string &file,
                                const std::vector<std::string>& allowed) {
    std::string ext = get_extension(file);
    if (std::find(allowed.begin(), allowed.end(), ext) == allowed.end()) {
        std::cerr << "Error: Unsupported file extension: " << ext << std::endl;
        return false;
    }
    return true;
}

#endif // HELPER_FUNCTIONS_HPP
