#pragma once

#include <iostream>
#include <cassert>

class Texture
{
 public:
    explicit Texture(const std::string & path);

    void Load();
    
    std::string path;
    int width;
    int height;
    int channels;

    unsigned char * data;
};
