#pragma once
#include "../String/string.h"

struct Texture
{
    unsigned int width, height, channels;
    unsigned int ID, index;

    static int GlobalTextureIndex;

    String path;
};