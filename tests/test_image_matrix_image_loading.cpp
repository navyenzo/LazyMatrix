//-------------------------------------------------------------------
#include <iostream>
#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("ImageMatrix Loading and Pixel Access", "[ImageMatrix]")
{
    // Generate a 2x2 test image
    dlib::matrix<dlib::rgb_pixel> test_image(2, 2);
    test_image(0, 0) = dlib::rgb_pixel(255, 0, 0); // Red
    test_image(0, 1) = dlib::rgb_pixel(0, 255, 0); // Green
    test_image(1, 0) = dlib::rgb_pixel(0, 0, 255); // Blue
    test_image(1, 1) = dlib::rgb_pixel(255, 255, 255); // White

    // Save to a temporary file
    std::string temp_filename = "temp_test_image.bmp";
    dlib::save_bmp(test_image, temp_filename);

    // Load the image using ImageMatrix
    LazyMatrix::ImageMatrix<dlib::rgb_pixel> img(temp_filename);

    // Test pixel values
    REQUIRE(img.at(0, 0) == dlib::rgb_pixel(255, 0, 0)); // Red
    REQUIRE(img.at(0, 1) == dlib::rgb_pixel(0, 255, 0)); // Green
    REQUIRE(img.at(1, 0) == dlib::rgb_pixel(0, 0, 255)); // Blue
    REQUIRE(img.at(1, 1) == dlib::rgb_pixel(255, 255, 255)); // White

    // Clean up: Delete the temporary file
    std::remove(temp_filename.c_str());
}
//-------------------------------------------------------------------