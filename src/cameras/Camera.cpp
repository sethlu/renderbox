#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


namespace renderbox {

    void Camera::didTransform() {
        Object::didTransform();
        viewMatrix = glm::inverse(worldMatrix);
        viewMatrixDidUpdate();
    }

    void Camera::projectionMatrixDidUpdate() {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void Camera::viewMatrixDidUpdate() {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    glm::mat4x4 Camera::getViewProjectionMatrix() const {
        return viewProjectionMatrix;
    };

    glm::mat4x4 Camera::getProjectionMatrix() const {
        return projectionMatrix;
    };

    glm::mat4x4 Camera::getViewMatrix() const {
        return viewMatrix;
    };

}
