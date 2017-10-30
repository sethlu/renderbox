#ifndef VOX_CAMERA_H
#define VOX_CAMERA_H


#include <glm/mat4x4.hpp>
#include "Object.h"
#include "Ray.h"

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

#endif //VOX_CAMERA_H
