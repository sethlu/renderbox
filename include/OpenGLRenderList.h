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
#include "PointLight.h"

namespace renderbox {

    struct OpenGLRenderList {

        std::unordered_map<Material *, std::vector<Object *>> objects;

        void addObject(Material *material, Object *object);

        std::vector<PointLight *> pointLights;

        void addLight(Light *light);

    };

}


#endif //VOX_OPENGLRENDERLIST_H
