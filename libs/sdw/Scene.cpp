#include "Scene.h"
#include <string>
#include "Utils.h"

Scene::Scene() {
    camera = Camera();
}

Scene::Scene(Camera camera, std::string objPath, float scale) {
    Scene::camera = camera;
    triangles = loadObj(objPath, scale);    
}