#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

/**
 * @brief Extracts the directory portion of a file path.
 *
 * This function finds the last occurrence of the '/' character in the given path 
 * and returns everything before that character. This is useful when you need 
 * to determine the folder in which a file is located, for instance, to ensure 
 * that the directory exists or to create it if it does not.
 *
 * @param path The full path to a file or directory.
 * @return A string representing the directory portion of the path. If no '/' is found,
 *         it returns ".", indicating the current directory.
 */
inline std::string extract_directory(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        return ".";
    return path.substr(0, pos);
}

/**
 * @brief Ensures that the given path ends with a '/' character.
 *
 * Many file operations require that directory paths have a trailing slash 
 * so that they can be concatenated with filenames without missing a separator.
 * This function checks if the provided path already ends with '/', and if not,
 * it appends one.
 *
 * @param path The original directory path.
 * @return A directory path that is guaranteed to end with a '/'.
 */
inline std::string ensure_trailing_slash(const std::string &path) {
    if (path.empty() || path.back() == '/')
        return path;
    return path + "/";
}

/**
 * @brief Extracts the file extension (without the dot) from a filename.
 *
 * This function locates the last occurrence of the '.' character in the filename 
 * and returns the substring that follows it. File extensions are often used to 
 * determine the file format (e.g., "txt", "dat", "h5"). This function is a common 
 * utility when you want to branch your logic based on the file format.
 *
 * @param filename The name of the file (can be a full path).
 * @return The file extension (without the leading dot). If no dot is found, returns an empty string.
 */
inline std::string get_extension(const std::string &filename) {
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos)
        return "";
    return filename.substr(pos + 1);
}

/**
 * @brief Validates that a file's extension is among a set of allowed extensions.
 *
 * This function checks the file's extension (obtained via get_extension()) against 
 * a provided list of allowed extensions. It is useful for ensuring that only supported 
 * file formats are processed by the application.
 *
 * @param file The filename to validate.
 * @param allowed A vector of strings representing allowed file extensions.
 * @return true if the file's extension is in the allowed list; false otherwise.
 *
 * @note This function only validates a single file. If you need to validate multiple 
 *       files and/or ensure they have the same extension, you would need to extend 
 *       this function accordingly.
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