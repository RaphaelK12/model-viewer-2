#pragma once
#include "../AssetManagement/asset_loader.h"

struct Model
{
    MeshIndexed mesh;
    Texture diffuse;
    Texture normal;
    std::string name;
};