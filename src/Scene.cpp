#include "Scene.h"
#include <string>
#include "Utils.h"

Scene::Scene() {
    camera = Camera();
}

Scene::Scene(Camera c, std::string objPath) {
    Scene::camera = c;
    triangles = loadObj(objPath, 0.17);    
}