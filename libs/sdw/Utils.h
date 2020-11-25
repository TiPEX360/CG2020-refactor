#pragma once

#include <string>
#include <vector>
#include "ModelTriangle.h"
#include "CanvasPoint.h"
#include "Material.h"

std::vector<std::string> split(const std::string &line, char delimiter);

std::vector<ModelTriangle> loadObj(std::string path, float scale);

Material loadMaterial(std::string mtlName, std::string path);

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues);
std::vector<glm::vec3> interpolateVector(glm::vec3 from, glm::vec3 to, int numberOfValues);
std::vector<glm::vec2> interpolateVector(glm::vec2 from, glm::vec2 to, int numberOfValues);
std::vector<CanvasPoint> interpolateVector(CanvasPoint from, CanvasPoint to, int numberOfValues);
uint32_t colourPack(Colour colour, int alpha);
uint32_t colourPack(glm::vec3 colour, int alpha);
