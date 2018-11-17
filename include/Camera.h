#ifndef RENDERBOX_CAMERA_H
#define RENDERBOX_CAMERA_H


#include "Object.h"
#include "Ray.h"

namespace renderbox {

    class Camera : public Object {
    public:

        virtual ~Camera() = default;

        OBJECT_TYPE getObjectType() const override {
            return CAMERA_OBJECT;
        };

        glm::mat4x4 getViewProjectionMatrix() const;

        glm::mat4x4 getProjectionMatrix() const;

        glm::mat4x4 getViewMatrix() const;

        virtual std::shared_ptr<Ray> getRay(glm::vec2 coordinates = glm::vec2(0)) const = 0;

    protected:

        glm::mat4x4 viewProjectionMatrix;

        glm::mat4x4 projectionMatrix;

        glm::mat4x4 viewMatrix;

        void didTransform() override;

        void projectionMatrixDidUpdate();

        void viewMatrixDidUpdate();

    };

}


#endif //RENDERBOX_CAMERA_H
