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
// Define every thing within the namespace Files
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to get the absolute path of the executable
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
        return fs::path(std::string(path, (count > 0) ? count: 0));
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to get the absolute directory path of the executable
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
        return fs::path(std::string(path, (count > 0) ? count: 0)).parent_path().string();
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to generate a unique filename in a specified
// directory
//-------------------------------------------------------------------
inline fs::path generate_unique_filename(const std::string filename_template = "XXXXXX",
                                         const fs::path& directory_where_file_will_reside = fs::temp_directory_path())
{
    #ifdef _WIN32
        fs::path unique_filename_path = directory_where_file_will_reside;
        unique_filename_path += filename_template;
        std::string unique_filename_string = unique_filename_path.string();
        _mktemp_s(unique_filename_string.data(), unique_filename_string.size() + 1);
        unique_filename_path = unique_filename_string;
        return unique_filename_path;
    #else
        fs::path unique_filename_path = std::string(fs::path(std::tmpnam(nullptr)));
        return unique_filename_path;
    #endif
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to create a file with a specified size with a unique
// filename based on a template in a user specified directory
// - This function first generates a unique filename, then creates
//   the file with the specified size and then returns and error code
// - The function returns the absolute path to the created file
//-------------------------------------------------------------------
inline fs::path create_file_with_specified_size_and_unique_name(std::size_t desired_file_size,
                                                                std::error_code& file_creation_error,
                                                                const std::string filename_template = "XXXXXX",
                                                                const fs::path& directory_where_file_will_reside = fs::temp_directory_path())
{
    // First we generate the unique name in the user
    // specified directory
    fs::path filename = generate_unique_filename(filename_template, directory_where_file_will_reside);

    // Create the file
    std::fstream file_to_create(filename.string(), std::fstream::in | std::fstream::out | std::fstream::trunc);

    if(!file_to_create.is_open())
    {
        file_creation_error.assign(1,std::iostream_category());
        return filename;
    }
    
    file_to_create.close();

    // Resize the file to the correct size
    fs::resize_file(filename, desired_file_size, file_creation_error);

    return filename;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_FILES_HPP_