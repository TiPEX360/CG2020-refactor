#include "Camera.h"

Camera::Camera() {
    Camera::pos = glm::vec3(0);
    Camera::rot = glm::mat3(1);
    Camera::focalLength = 2.0f;
}

Camera::Camera(glm::vec3 pos, glm::mat3 rot, float focalLength) {
    Camera::pos = pos;
    Camera::rot = rot;
    Camera::focalLength = focalLength;
}