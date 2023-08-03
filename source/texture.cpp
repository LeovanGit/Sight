#include <iostream>

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"

Texture::Texture(const std::string & path) : path(path)
{
    Load();
}

void Texture::Load()
{
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    assert(data != nullptr && "stbi_load() can't load image");

#ifdef LOGS
    std::cout << "Loading sight texture: " << width << "x" << height << "\n";
#endif
}
