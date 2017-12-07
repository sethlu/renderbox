#ifndef VOX_OPENGLRENDERLIST_H
#define VOX_OPENGLRENDERLIST_H


#include <glm/mat4x4.hpp>
#include <vector>
#include <unordered_map>
#include "OpenGLVertexArray.h"
#include "Object.h"
#include "ObjectProperties.h"
#include "OpenGLObjectProperties.h"
#include "Scene.h"

namespace renderbox {

    struct OpenGLRenderList {

        /**
         * Map from OpenGLProgram id to a list of objects using it
         */
        std::unordered_map<GLuint, std::vector<Object *>> objects;

        void addObject(GLuint programId, Object *object);

    };

}


#endif //VOX_OPENGLRENDERLIST_H
