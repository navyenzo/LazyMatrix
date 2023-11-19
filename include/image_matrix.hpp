// Include the necessary headers
#ifndef INCLUDE_IMAGE_MATRIX_HPP_
#define INCLUDE_IMAGE_MATRIX_HPP_
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include "stb_image.h"

#include "base_matrix.hpp"
#include "color.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename ColorType>
class ImageMatrix : public BaseMatrix<ImageMatrix<ColorType> >
{
public:

    ImageMatrix()
    {
    }

    ImageMatrix(const char* filename)
    {
        this->load(filename);
    }

    ~ImageMatrix()
    {
        this->free();
    }



    void load(const char* filename)
    {
        this->free();


        int width, height, channels;
        data_ = stbi_load(filename, &width, &height, &channels, 0);
        
        if (data_)
        {
            rows_ = height;
            columns_ = width;  // now columns_ represents width in pixels
            channels_ = channels;

            // image successfully loaded
            return;
        }
        
        float_data_ = stbi_loadf(filename, &width, &height, &channels, 0);

        if (float_data_)
        {
            rows_ = height;
            columns_ = width;  // now columns_ represents width in pixels
            channels_ = channels;

            // image successfully loaded
            return;
        }
    }

    void free()
    {
        rows_ = 0;
        columns_ = 0;

        if(data_)
        {
            stbi_image_free(data_);
        }
        
        if(float_data_)
        {
            stbi_image_free(float_data_);
        }

        data_ = nullptr;
        float_data_ = nullptr;
    }



    int64_t rows() const
    {
        return rows_;
    }

    int64_t columns() const
    {
        return columns_;
    }

    int64_t channels() const
    {
        return channels_;
    }

    

    ColorType at(int64_t row, int64_t column) const
    {
        int64_t index = (row * columns_ + column) * channels_;

        int number_of_channels = get_number_of_channels<ColorType>();

        if constexpr (is_type_a_color<ColorType>::value)
        {
            if(data_)
            {
                switch(channels_)
                {
                    default:
                    case 0:
                        return ColorType(0);
                    case 1:
                        return ColorType(data_[index]);
                    case 2:
                        return ColorType(data_[index], data_[index + 1]);
                    case 3:
                        return ColorType(data_[index], data_[index + 1], data_[index + 2]);
                    case 4:
                        return ColorType(data_[index], data_[index + 1], data_[index + 2], data_[index + 3]);
                }
            }
            else
            {
                switch(channels_)
                {
                    default:
                    case 0:
                        return ColorType(0);
                    case 1:
                        return ColorType(float_data_[index]);
                    case 2:
                        return ColorType(float_data_[index], float_data_[index + 1]);
                    case 3:
                        return ColorType(float_data_[index], float_data_[index + 1], float_data_[index + 2]);
                    case 4:
                        return ColorType(float_data_[index], float_data_[index + 1], float_data_[index + 2], float_data_[index + 3]);
                }
            }
        }
        else if constexpr (is_complex<ColorType>::value)
        {
            if(data_)
            {
                switch(channels_)
                {
                    default:
                    case 0:
                        return ColorType(0);
                    case 1:
                        return ColorType(data_[index]);
                    case 2:
                        return ColorType(data_[index], data_[index + 1]);
                }
            }
            else
            {
                switch(channels_)
                {
                    default:
                    case 0:
                        return ColorType(0);
                    case 1:
                        return ColorType(float_data_[index]);
                    case 2:
                        return ColorType(float_data_[index], float_data_[index + 1]);
                }
            }
        }
        else
        {
            if(data_)
            {
                switch(channels_)
                {
                    case 0:
                        return ColorType(0);
                    default:
                        return data_[index];
                }
            }
            else
            {
                switch(channels_)
                {
                    case 0:
                        return ColorType(0);
                    default:
                        return float_data_[index];
                }
            }
        }
    }



    ColorType at(int64_t index) const
    {
        int64_t row = index / columns_;
        int64_t column = index % columns_;
        return at(row, column);
    }



private:

    unsigned char* data_ = nullptr;
    float* float_data_ = nullptr;
    int64_t rows_ = 0;
    int64_t columns_ = 0;
    int channels_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ColorType>

struct is_type_a_matrix< ImageMatrix<ColorType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
}  // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_IMAGE_MATRIX_HPP_