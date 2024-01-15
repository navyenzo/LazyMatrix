//-------------------------------------------------------------------
/**
 * @file test_file_utils.cpp
 * @brief Test file for file and directory utilities in the LazyMatrix library.
 *
 * This file contains tests for the file and directory utility functions
 * provided in the LazyMatrix library, ensuring their correctness and reliability.
 * Specifically, it tests the functionality of listing files matching a specific name or pattern.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace fs = std::filesystem;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("List files matching a specific name or pattern", "[file_utils]")
{
    // Create a temporary directory for testing
    // and fill it with some files
    fs::path temp_dir = fs::temp_directory_path() / "LazyMatrixTestDir";
    fs::create_directory(temp_dir);

    // Create some temporary .ttf files
    std::vector<fs::path> created_files;
    for (int i = 0; i < 5; ++i)
    {
        fs::path temp_file = temp_dir / ("test_" + std::to_string(i) + ".ttf");
        std::ofstream(temp_file).close(); // Create an empty file
        created_files.push_back(temp_file);
    }

    // Create a non-.ttf file for control
    fs::path non_ttf_file = temp_dir / "test_non_ttf.txt";
    std::ofstream(non_ttf_file).close();

    SECTION("List .ttf files in the given directory")
    {
        auto matching_files = LazyMatrix::list_files_matching_name(temp_dir, ".ttf");

        // Check if the returned vector contains all the .ttf files created
        REQUIRE(matching_files.size() == created_files.size());
        for (const auto& file : matching_files)
        {
            REQUIRE(file.extension() == ".ttf");
            REQUIRE(std::find(created_files.begin(), created_files.end(), file) != created_files.end());
        }
    }

    // Clean up: Remove the created files and directory
    for (const auto& file : created_files)
    {
        fs::remove(file);
    }
    fs::remove(non_ttf_file);
    fs::remove(temp_dir);
}
//-------------------------------------------------------------------