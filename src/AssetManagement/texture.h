#pragma once
#include <string>

struct Texture
{
    unsigned int width, height, channels;
    unsigned int ID, index;

    static int GlobalTextureIndex;

    std::string path;
};