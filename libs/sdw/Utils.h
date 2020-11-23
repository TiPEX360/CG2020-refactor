#pragma once

#include <string>
#include <vector>
#include "ModelTriangle.h"
#include "Material.h"

std::vector<std::string> split(const std::string &line, char delimiter);

std::vector<ModelTriangle> loadObj(std::string path, float scale);

Material loadMaterial(std::string mtlName, std::string path);