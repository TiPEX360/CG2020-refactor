#pragma once

#include <string>
#include <vector>
#include "ModelTriangle.h"
#include "Material.h"

std::vector<std::string> split(const std::string &line, char delimiter);

std::vector<ModelTriangle> loadObj(string path, float scale);

std::vector<Material> loadMtl(string mtlName, string path);