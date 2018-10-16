#ifndef RENDERBOX_OPENGLRENDERLIST_H
#define RENDERBOX_OPENGLRENDERLIST_H


#include <vector>
#include <unordered_map>

#include "Object.h"
#include "ObjectProperties.h"
#include "PointLight.h"
#include "Scene.h"

namespace renderbox {

    struct MetalRenderList {

        std::unordered_map<Material *, std::vector<Object *>> objects;

        void addObject(Material *material, Object *object);

        std::vector<PointLight *> pointLights;

        void addLight(Light *light);

    };

}


#endif //RENDERBOX_OPENGLRENDERLIST_H
