#pragma once

struct Texture
{
    unsigned int width, height, channels;
    unsigned int ID, index;

    static int GlobalTextureIndex;
};