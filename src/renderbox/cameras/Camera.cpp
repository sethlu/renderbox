#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


namespace renderbox {

    glm::mat4x4 Camera::getViewProjectionMatrix() {
        return projectionMatrix * getViewMatrix();
    }

    glm::mat4x4 Camera::getProjectionMatrix() {
        return projectionMatrix;
    }

//    void Camera::lookAt(glm::vec3 position, glm::vec3 up) {
//        viewMatrix = glm::lookAt(getWorldPosition(), position, up);
//    }

    glm::mat4x4 Camera::getViewMatrix() {
//        return glm::lookAt(getWorldPosition(), glm::vec3(0.0f), glm::vec3(0, 0, 1.0f));
        return glm::inverse(worldMatrix);
    }

    Ray *Camera::getRay(glm::vec2) {
        return nullptr;
    }

}
