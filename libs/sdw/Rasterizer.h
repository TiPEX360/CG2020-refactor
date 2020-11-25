#pragma once

#include <vector>
#include "Scene.h"
#include "DrawingWindow.h"
#include "Renderer.h"

class Rasterizer : public Renderer {
public:
    Scene* scene;
    DrawingWindow window;

    Rasterizer(DrawingWindow window, Scene* scene);
    void render();
private:
    int width;
    int height;
    std::vector<std::vector<float>> ZBuffer;

    void drawModelTriangle(ModelTriangle triangle);
};