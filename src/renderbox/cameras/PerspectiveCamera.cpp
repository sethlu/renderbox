#include <glm/gtc/matrix_transform.hpp>
#include "PerspectiveCamera.h"
#include "../renderbox.h"


namespace renderbox {

    PerspectiveCamera::PerspectiveCamera(float fovy, float aspectRatio, float zNear, float zFar) {
        setPerspective(fovy, aspectRatio, zNear, zFar);
    }

    void PerspectiveCamera::setPerspective(float fovy, float aspectRatio, float zNear, float zFar) {
        projectionMatrix = glm::perspective(fovy, aspectRatio, zNear, zFar);
    }

    Ray *PerspectiveCamera::getRay(glm::vec2 coordinates) {
        glm::vec3 origin = getWorldPosition();
        glm::vec3 testVector = dehomogenize(glm::inverse(getViewProjectionMatrix())
                                            * glm::vec4(coordinates, -1.0f, 1.0f));
        return new Ray(origin, glm::normalize(testVector - origin));
    }

}