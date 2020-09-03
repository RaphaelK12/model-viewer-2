#pragma once
#include "../AssetManagement/asset_loader.h"

struct Model
{
    Mesh mesh;
    Texture diffuse;
    Texture normal;
    Texture specular;
    std::string name;
};