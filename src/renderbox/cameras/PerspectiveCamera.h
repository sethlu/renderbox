#include <glm/glm.hpp>
#include "Camera.h"
#include "../math/Ray.h"


#ifndef PARTICLESIM_PERSPECTIVECAMERA_H
#define PARTICLESIM_PERSPECTIVECAMERA_H


namespace renderbox {

    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(float fovy, float aspectRatio, float zNear = 0.1f, float zFar = 10000.0f);
        void setPerspective(float fovy, float aspectRatio, float zNear = 0.1f, float zFar = 10000.0f);
        Ray *getRay(glm::vec2 coordinates = glm::vec2(0.0f));
    };

}


#endif //PARTICLESIM_PERSPECTIVECAMERA_H
