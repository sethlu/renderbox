#ifndef RENDERBOX_PERSPECTIVECAMERA_H
#define RENDERBOX_PERSPECTIVECAMERA_H


#include "Camera.h"
#include "Ray.h"

namespace renderbox {

    class PerspectiveCamera : public Camera {
    public:

        PerspectiveCamera(float fovy, float aspectRatio, float zNear = 0.1f, float zFar = 100.0f);

        ~PerspectiveCamera() override = default;

        void setPerspective(float fovy, float aspectRatio, float zNear = 0.1f, float zFar = 100.0f);

        Ray *getRay(glm::vec2 coordinates = glm::vec2(0)) const override;

    };

}


#endif //RENDERBOX_PERSPECTIVECAMERA_H
