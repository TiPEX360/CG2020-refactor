#include "Rasterizer.h"
#include "CanvasPoint.h"
#include "CanvasTriangle.h"

void drawFilledTriangle(CanvasTriangle triangle, Colour colour) {

}

void drawMaterialTriangle(CanvasTriangle triangle, Material material) {
    
}

void Rasterizer::drawModelTriangle(ModelTriangle triangle) {
    std::array<CanvasPoint, 3> cPoints;
    for(int i = 0; i < 3; i++) {
        CanvasPoint p = CanvasPoint();
        glm::vec3 vertex = triangle.vertices[i] - scene->camera.pos;
        vertex = scene->camera.rot * vertex;
        p.x = glm::floor(-1*scene->camera.focalLength*(vertex.x / vertex.z) + width/2);
        p.y = glm::floor(scene->camera.focalLength*(vertex.y / vertex.z) + height/2);
        if(vertex.z != 0.0) p.depth = glm::abs(1 / vertex.z);
        else p.depth = INFINITY;
        cPoints[i] = p;
    } 

    CanvasTriangle cTriangle = CanvasTriangle(cPoints[0], cPoints[1], cPoints[2]);

    //If proper material not present use colour
    if(triangle.material.texturePath.empty()) drawFilledTriangle(cTriangle, triangle.colour);
    else drawMaterialTriangle(cTriangle, triangle.material);
}

void Rasterizer::render() {
    for(int i = 0; i < scene->triangles.size(); i++) {
        drawModelTriangle(scene->triangles[i]);
    }
}

Rasterizer::Rasterizer(DrawingWindow window, Scene* scene) {
    Rasterizer:: window = window;
    Rasterizer::scene = scene;
    Rasterizer::width = window.width;
    Rasterizer::height = window.height;
}