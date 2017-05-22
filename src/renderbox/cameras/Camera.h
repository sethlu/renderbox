#ifndef PARTICLESIM_CAMERA_H
#define PARTICLESIM_CAMERA_H


#include <glm/mat4x4.hpp>
#include "../core/Object.h"
#include "../math/Ray.h"

namespace renderbox {

    class Camera : public Object {
    protected:
        glm::mat4x4 viewProjectionMatrix;
        glm::mat4x4 projectionMatrix;
        glm::mat4x4 viewMatrix;
        void didTransform();
        void projectionMatrixDidUpdate();
        void viewMatrixDidUpdate();
    public:
        glm::mat4x4 getViewProjectionMatrix();
        glm::mat4x4 getProjectionMatrix();
        glm::mat4x4 getViewMatrix();
        virtual Ray *getRay(glm::vec2 coordinates = glm::vec2(0.0f));
    };

}

#endif //PARTICLESIM_CAMERA_H
