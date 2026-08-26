#pragma once
#include <vector>
#include <cstdint>
#include <glad/glad.h>
#include <iostream>

bool loadBMP(const char* path, int& width, int& height, std::vector<uint8_t>& outPixels);

inline unsigned int loadTextureFromBMP(const char* path) {
    int width, height;
    std::vector<uint8_t> pixels;

    if (!loadBMP(path, width, height, pixels)) {
        std::cerr << "I HAVE FAILED TO LOAD THE FUCKING TEXTURE: " << path << std::endl;
        return 0;
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
