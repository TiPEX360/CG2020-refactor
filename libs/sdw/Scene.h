#pragma once

#include <vector>
#include <string>
#include "Utils.h"
#include "Camera.h"

class Scene {
public:
    Camera camera;
    std::vector<ModelTriangle> triangles;
    Scene();
    Scene(Camera camera, std::string objPath, float scale);
private:
};