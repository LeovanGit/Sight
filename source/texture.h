#pragma once

#include <iostream>
#include <cassert>

class Texture
{
 public:
    explicit Texture(const std::string & path);

    void load(const std::string & path);
    
    const unsigned char * getData() const;
    
    int getWidth() const;
    
    int getHeight() const;
    
    int getChannels() const;

    uint32_t getSize() const;
    
    ~Texture();

    // temporarily unavailable
    // deleted methods should be public for better error messages
    Texture(Texture &) = delete;
    Texture(Texture &&) = delete;
    void operator=(Texture &) = delete;
    void operator=(Texture &&) = delete;

private:    
    std::string path;
    
    int width;
    int height;
    int channels;

    // 32 bits per pixel, so 8 bits for pixel component
    unsigned char * data;
};
