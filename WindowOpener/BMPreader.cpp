#include "bmp_loader.h"
#include <fstream>
#include <iostream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };
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
    uint16_t bit_count{ 0 };
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 };
    int32_t  x_pixels_per_meter{ 0 };
    int32_t  y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };
    uint32_t colors_important{ 0 };
};
#pragma pack(pop)

bool loadBMP(const char* path, int& width, int& height, std::vector<uint8_t>& outPixels) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Can't open file!" << std::endl;
        return false;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    //read the data streams using .read() and type casting to char*
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    // chekcs for the bmp signature
    if (fileHeader.file_type != 0x4D42) { // hex code is BM
        std::cerr << "THIS IS NOT A BMP FILE!!!!" << std::endl;
        return false;
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

    std::vector<uint8_t> flipped(pixels.size());  //flips the row order so it reads the image correctly so its not upside down.

    int rowSize = infoHeader.width * 3; //rowSize can only be declared once, and i did it here, pretty obvious but i made the mistake.

    for (int y = 0; y < infoHeader.height; y++) {
        int srcRow = y; //like this, the image is not upside down.

        int srcOffset = srcRow * rowSize;                   //y coordinate 
        int dstOffset = y * rowSize;

        for (int x = 0; x < infoHeader.width; x++) {
            int srcPixelOffset = srcOffset + (x * 3);       //x coordinate 
            int dstPixelOffset = dstOffset + (x * 3);

            flipped[dstPixelOffset + 0] = pixels[srcPixelOffset + 2];
            flipped[dstPixelOffset + 1] = pixels[srcPixelOffset + 1];
            flipped[dstPixelOffset + 2] = pixels[srcPixelOffset + 0];

        }
    }

    std::cout << "Offset: " << fileHeader.offset_data << std::endl;

    std::cout << "Row size: " << rowSize << std::endl;

    std::cout << "First pixel (BGR): "
        << (int)pixels[0] << ", "
        << (int)pixels[1] << ", "
        << (int)pixels[2] << std::endl;


    width = infoHeader.width;
    height = infoHeader.height;
    outPixels = flipped;

	std::cout << "general info: " << infoHeader.width << "x" << infoHeader.height << " @" << infoHeader.bit_count << "bpp" << std::endl;

    return true;
}




  