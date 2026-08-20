#pragma once
#include <vector>
#include <cstdint>

bool loadBMP(const char* path, int& width, int& height, std::vector<uint8_t>& outPixels);
