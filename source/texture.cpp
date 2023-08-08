#include <iostream>

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb_image.h"

Texture::Texture(const std::string & path) :
                 path(path),
                 data(nullptr)
{
    load(path);
}

void Texture::load(const std::string & path)
{
    if (data) stbi_image_free(data);

    this->path = path;
    
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    assert(data != nullptr && "stbi_load() can't load image");

#ifdef LOGS
    std::cout << "Loading crosshair texture " << width << "x" << height << " from " << path << "\n";
#endif
}

const unsigned char * Texture::getData() const { return data; }

const int Texture::getWidth() const { return width; }

const int Texture::getHeight() const { return height; }

const int Texture::getChannels() const { return channels; }

const uint32_t Texture::getSize() const
{
    return width * height * channels;
}

Texture::~Texture() { stbi_image_free(data); }
