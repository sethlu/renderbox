#include <glm/mat4x4.hpp>
#include "../core/Object.h"
#include "../math/Ray.h"


#ifndef PARTICLESIM_CAMERA_H
#define PARTICLESIM_CAMERA_H


namespace renderbox {

    class Camera : public Object {
    protected:
        glm::mat4x4 projectionMatrix;
        // glm::mat4x4 viewMatrix;
    public:
        glm::mat4x4 getViewProjectionMatrix();
        glm::mat4x4 getProjectionMatrix();
        glm::mat4x4 getViewMatrix();
        // void lookAt(glm::vec3 position, glm::vec3 up = glm::vec3(1.0f, 0, 0));
        virtual Ray *getRay(glm::vec2 coordinates = glm::vec2(0.0f));
    };

}

#endif //PARTICLESIM_CAMERA_H
