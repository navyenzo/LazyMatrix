//-------------------------------------------------------------------
/**
 * @file files.hpp
 * @brief File and directory utilities for the LazyMatrix library.
 *
 * Provides functions for handling files and directories, including
 * obtaining the executable's path, creating uniquely named files,
 * and managing file sizes. These utilities are essential for
 * operations involving file I/O in the LazyMatrix library.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_FILES_HPP_
#define INCLUDE_FILES_HPP_



//-------------------------------------------------------------------
#ifdef _WIN32
    #include <io.h>
#endif

#include <system_error>
#include <fstream>

#if __has_include("filesystem")
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

#ifdef __linux__
    #include <unistd.h>
#else
    #include <libloaderapi.h>
#endif
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Get the absolute path of the executable.
 * 
 * @return fs::path Absolute path of the executable.
 */
//-------------------------------------------------------------------
inline fs::path get_absolute_path_of_executable()
{
    #ifdef _WIN32
        wchar_t path[FILENAME_MAX] = { 0 };
        GetModuleFileNameW(nullptr, path, FILENAME_MAX);
        return fs::path(path);
    #else
        char path[FILENAME_MAX];
        ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
        return fs::path(std::string(path, (count > 0) ? count : 0));
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Get the absolute directory path of the executable's parent directory.
 * 
 * @return fs::path Absolute path of the executable's parent directory.
 */
//-------------------------------------------------------------------
inline fs::path get_absolute_path_of_executable_parent_directory()
{
    #ifdef _WIN32
        wchar_t path[FILENAME_MAX] = { 0 };
        GetModuleFileNameW(nullptr, path, FILENAME_MAX);
        return fs::path(path).parent_path().string();
    #else
        char path[FILENAME_MAX];
        ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
        return fs::path(std::string(path, (count > 0) ? count : 0)).parent_path().string();
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Create a file with a specified size and unique name based on a template in a specified directory.
 * 
 * This function generates a unique filename, creates the file with the specified size, 
 * and returns an error code. It handles the creation of the file differently on Windows and Linux.
 * 
 * @param desired_file_size Size of the file to be created.
 * @param file_creation_error Error code for file creation.
 * @param filename_template Template for the filename, should end with "XXXXXX".
 * @param directory_where_file_will_reside Directory where the file will reside.
 * @return fs::path Path to the created file.
 */
//-------------------------------------------------------------------
inline fs::path create_file_with_specified_size_and_unique_name(
    std::size_t desired_file_size,
    std::error_code& file_creation_error,
    std::string filename_template = "XXXXXX",
    const fs::path& directory_where_file_will_reside = fs::temp_directory_path()) {

    // Ensure the filename template ends with exactly "XXXXXX"
    const size_t x_count = 6;
    size_t found_x = filename_template.find_last_not_of('X');
    size_t num_x_to_append = (found_x == std::string::npos) ? x_count : std::min(x_count, x_count - (filename_template.length() - found_x - 1));
    filename_template.append(num_x_to_append, 'X');

    fs::path filename; // Variable to store the generated filename

    #ifdef _WIN32
        // Construct the full path for the unique filename in Windows
        fs::path unique_filename_path = directory_where_file_will_reside / filename_template;
        std::string unique_filename_string = unique_filename_path.string();

        // Generate a unique filename
        _mktemp_s(unique_filename_string.data(), unique_filename_string.size() + 1);
        filename = unique_filename_string; // Update filename with the generated unique filename

        // Create and open the file to ensure it exists
        std::ofstream file_stream(filename, std::ios::binary | std::ios::out);
        if (!file_stream) {
            file_creation_error.assign(errno, std::generic_category());
            return fs::path(); // Return an empty path on failure
        }
        file_stream.close();
    #else
        // Construct the full template path for unique filename in Linux
        std::string full_template = (directory_where_file_will_reside / filename_template).string();

        // Create and open a unique temporary file using mkstemp
        int fd = mkstemp(&full_template[0]);
        if (fd != -1) {
            close(fd); // Close the file descriptor as we only need the unique filename
            filename = fs::path(full_template); // Update filename with the generated unique filename
        } else {
            // Handle errors in file creation
            file_creation_error.assign(errno, std::generic_category());
            return fs::path(); // Return an empty path on failure
        }
    #endif

    // Resize the file to the desired size
    fs::resize_file(filename, desired_file_size, file_creation_error);

    // Additional check to confirm the file size is as expected
    if (!file_creation_error && fs::file_size(filename) != desired_file_size) {
        file_creation_error.assign(std::make_error_code(std::errc::io_error).value(), std::generic_category());
    }

    return filename; // Return the path of the created file
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_FILES_HPP_
