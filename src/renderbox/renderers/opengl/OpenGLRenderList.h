#ifndef PARTICLESIM_OPENGLRENDERLIST_H
#define PARTICLESIM_OPENGLRENDERLIST_H


#include <glm/mat4x4.hpp>
#include <vector>
#include <unordered_map>
#include "OpenGLVertexArray.h"
#include "../../core/Object.h"
#include "../ObjectProperties.h"
#include "OpenGLObjectProperties.h"
#include "../../lights/Light.h"
#include "../../scenes/Scene.h"

namespace renderbox {

    struct OpenGLRenderList {

        /**
         * Map from OpenGLProgram id to a list of objects using it
         */
        std::unordered_map<GLuint, std::vector<Object *>> objects;

        void addObject(GLuint programID, Object *object);

        /**
         * Map from Light type to a list of lights
         */
        std::unordered_map<int, std::vector<Light *>> lights;

    };

}


#endif //PARTICLESIM_OPENGLRENDERLIST_H
