#pragma once

#include "Colour.h"

class Material {
public:
    Colour colour;
    std::string texturePath;
    
    Material();
    Material(Colour colour,  std::string texturePath);
};