#pragma once

#include <glm/glm.hpp>

class Camera {
private:
public: 
    glm::vec3 pos;
    glm::mat3 rot;
    float focalLength;

    Camera();
    Camera(glm::vec3 pos, glm::mat3 rot, float focalLengh);
};