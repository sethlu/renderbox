#ifndef RENDERBOX_OPENGLRENDERLIST_H
#define RENDERBOX_OPENGLRENDERLIST_H


#include <vector>
#include <unordered_map>

#include "Object.h"
#include "ObjectProperties.h"
#include "PointLight.h"
#include "Scene.h"
#include "MetalRenderPipelineState.h"

namespace renderbox {

    struct MetalRenderList {

        std::unordered_map<MetalRenderPipelineState *, std::vector<Object *>> objects;

        void addObject(MetalRenderPipelineState *renderPipelineState, Object *object);

        std::vector<PointLight *> pointLights;

        void addLight(Light *light);

    };

}


#endif //RENDERBOX_OPENGLRENDERLIST_H
