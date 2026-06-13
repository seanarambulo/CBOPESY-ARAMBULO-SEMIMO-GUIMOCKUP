#pragma once
#include <string>

class TextureLoader {
public:
    static unsigned int loadTexture(const std::string& filename, int& width, int& height);
};
