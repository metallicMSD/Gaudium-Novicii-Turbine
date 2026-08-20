#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

//as of now, its hardcoded to only work on 1 image, you need to replace the file directory to your
//bmp file with the full directory and it should probably work.
//i'll add a GUI later, if i can actually import the image into opengl.

// 1 here prevents padding
#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 }; // BM
    uint32_t file_size{ 0 };
    uint16_t reserved1{ 0 };
    uint16_t reserved2{ 0 };
    uint32_t offset_data{ 0 };   
};

struct BMPInfoHeader {
    uint32_t size{ 0 };
    int32_t  width{ 0 };
    int32_t  height{ 0 };
    uint16_t planes{ 1 };
    uint16_t bit_count{ 0 };      // Bits per pixel 
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 };
    int32_t  x_pixels_per_meter{ 0 };
    int32_t  y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };
    uint32_t colors_important{ 0 };
};
#pragma pack(pop)

int main() {

    std::cout << "Current working directory: "
        << std::filesystem::current_path() << std::endl;

    std::ifstream file("C:\\Users\\tallm\\source\\repos\\bmp reader\\bmp reader\\green_triangle.bmp", std::ios::binary);

    if (!file) {
        std::cerr << "CANT OPEN IT! FUCK!" << std::strerror(errno) << std::endl;
        return 1;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    //read the data streams using .read() and type casting to char*
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    // chekcs for the bmp signature
    if (fileHeader.file_type != 0x4D42) { // hex code is BM
        std::cerr << "THIS IS NOT A BMP FILE!!!!" << std::endl;
        return 1;
    }

    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    //prints the output
    std::cout << "Width: " << infoHeader.width << " pixels" << std::endl;
    std::cout << "Height: " << infoHeader.height << " pixels" << std::endl;
    std::cout << "Bit Depth: " << infoHeader.bit_count << " bpp" << std::endl;

    //read pixel data into vector.
    std::vector<uint8_t> pixels(infoHeader.size_image);
    file.seekg(fileHeader.offset_data); //move to the pixel data offset
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());

    std::cout << "Offset: " << fileHeader.offset_data << std::endl;
   
    int rowSize = infoHeader.width * 3;
    std::cout << "Row size: " << rowSize << std::endl;

    std::cout << "First pixel (BGR): "
        << (int)pixels[0] << ", "
        << (int)pixels[1] << ", "
        << (int)pixels[2] << std::endl;

    return 0;
}
