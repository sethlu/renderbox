#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


namespace renderbox {

    void Camera::didTransform() {
        Object::didTransform();
        viewMatrix = glm::inverse(worldMatrix);
        viewMatrixDidUpdate();
    }

    glm::mat4x4 Camera::getViewProjectionMatrix() {
        return viewProjectionMatrix;
    }

    glm::mat4x4 Camera::getProjectionMatrix() {
        return projectionMatrix;
    }

    glm::mat4x4 Camera::getViewMatrix() {
        return viewMatrix;
    }

    Ray *Camera::getRay(glm::vec2) {
        return nullptr;
    }

    void Camera::projectionMatrixDidUpdate() {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera::viewMatrixDidUpdate() {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

}
