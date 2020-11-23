#include 'Scene.h'

Scene::Scene() {
    camera = Camera();
}

Scene::Scene(Camera c, string objPath) {
    Scene::camera = c;
    triangles = loadObj(objPath);    
}