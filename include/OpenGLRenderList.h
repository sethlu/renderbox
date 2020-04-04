#ifndef RENDERBOX_OPENGLRENDERLIST_H
#define RENDERBOX_OPENGLRENDERLIST_H


#include <vector>
#include <unordered_map>

#include "Object.h"
#include "PointLight.h"


namespace renderbox {

    struct OpenGLRenderList {

        std::unordered_map<OpenGLProgram const *, std::vector<Object const *>> objects;

        void addObject(OpenGLProgram const *program, Object const *object);

        std::vector<PointLight *> pointLights;

        void addLight(Light *light);

    };

}


#endif //RENDERBOX_OPENGLRENDERLIST_H
