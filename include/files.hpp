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
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_FILES_HPP_
#define INCLUDE_FILES_HPP_



//-------------------------------------------------------------------
#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX // Prevents the Windows headers from defining min() and max() macros
    #endif
    #include <io.h>
    #include <Windows.h> // Make sure to include Windows.h after defining NOMINMAX
#endif

#include <system_error>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#if __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "No filesystem support"
#endif

#ifdef __linux__
    #include <unistd.h>
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
 * This function generates a unique filename with the specified extension, creates the file with the specified size, 
 * and returns an error code. It handles the creation of the file differently on Windows and Linux.
 * 
 * @param desired_file_size Size of the file to be created.
 * @param file_creation_error Error code for file creation.
 * @param filename_template Template for the filename, can include extension like "XXX.txt".
 * @param directory_where_file_will_reside Directory where the file will reside.
 * @return fs::path Path to the created file.
 */
//-------------------------------------------------------------------
inline fs::path create_file_with_specified_size_and_unique_name(
    std::size_t desired_file_size,
    std::error_code& file_creation_error,
    fs::path filename_template = "XXXXXX",
    const fs::path& directory_where_file_will_reside = fs::temp_directory_path())
{
    // If filename_template is not absolute, combine it with the specified directory
    if (!filename_template.is_absolute())
    {
        filename_template = directory_where_file_will_reside / filename_template.filename();
    }

    // Process the filename to insert 'X's before the extension
    const size_t x_count = 6;
    std::string filename_str = filename_template.stem().string(); // Extract filename without extension
    std::string extension_str = filename_template.extension().string(); // Extract extension

    size_t found_x = filename_str.find_last_not_of('X');
    //size_t num_x_to_append = (found_x == std::string::npos) ? x_count : std::min(x_count, x_count - (filename_str.length() - found_x - 1));
    size_t num_x_to_append = (found_x == std::string::npos) ? x_count : (std::min)(x_count, x_count - (filename_str.length() - found_x - 1));
    filename_str.append(num_x_to_append, 'X');

    // Reassemble the filename with the extension
    filename_template.replace_filename(filename_str + extension_str);

    // Validate and create directories if needed
    if (!fs::exists(filename_template.parent_path()))
    {
        fs::create_directories(filename_template.parent_path());
    }

    fs::path filename; // Variable to store the generated filename

    #ifdef _WIN32
        // Windows-specific file creation
        std::string unique_filename_string = filename_template.string();
        _mktemp_s(unique_filename_string.data(), unique_filename_string.size() + 1);
        filename = unique_filename_string;

        std::ofstream file_stream(filename, std::ios::binary | std::ios::out);
        if (!file_stream)
        {
            file_creation_error.assign(errno, std::generic_category());
            return fs::path();
        }
        file_stream.close();
    #else
        // Linux-specific file creation
        std::string full_template = filename_template.string();
        int fd = mkstemp(&full_template[0]);
        if (fd != -1)
        {
            close(fd);
            filename = fs::path(full_template);
        }
        else
        {
            file_creation_error.assign(errno, std::generic_category());
            return fs::path();
        }
    #endif

    // Resize the file to the desired size
    fs::resize_file(filename, desired_file_size, file_creation_error);

    if (!file_creation_error && fs::file_size(filename) != desired_file_size)
    {
        file_creation_error.assign(std::make_error_code(std::errc::io_error).value(), std::generic_category());
    }

    return filename;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief List all files in a directory and subdirectories matching a specific name or pattern.
 * 
 * @param directory The directory to search in.
 * @param name_to_match The name or pattern to match.
 * @return std::vector<fs::path> A vector of file paths that match the given name or pattern.
 */
//-------------------------------------------------------------------
inline std::vector<fs::path> list_files_matching_name(const fs::path& directory, const std::string& name_to_match)
{
    std::vector<fs::path> matching_files;
    std::error_code ec;

    // Check if directory exists and is indeed a directory
    if (!fs::exists(directory, ec) || !fs::is_directory(directory, ec))
        return matching_files;

    for (const auto& entry : fs::recursive_directory_iterator(directory, ec))
    {
        if (ec)
            break; // Stop if an error occurred during directory traversal

        if (entry.is_regular_file(ec) && entry.path().filename().string().find(name_to_match) != std::string::npos)
        {
            matching_files.push_back(entry.path());
        }

        if (ec)
            break; // Stop if an error occurred while accessing file properties
    }

    return matching_files;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_FILES_HPP_
