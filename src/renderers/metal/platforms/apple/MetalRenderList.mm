#include "MetalRenderList.h"


namespace renderbox {

    void MetalRenderList::addObject(MetalRenderPipelineState *renderPipelineState, Object *object) {
        auto result = objects.find(renderPipelineState);
        if (result != objects.end()) {
            result->second.insert(result->second.end(), object);
        } else {
            std::vector<Object *> objectList;
            objectList.insert(objectList.end(), object);
            objects.insert(std::make_pair(renderPipelineState, objectList));
        }
    }

    void MetalRenderList::addLight(Light *light) {
        switch (light->getLightType()) {
            default: break;
            case POINT_LIGHT: pointLights.emplace_back(static_cast<PointLight *>(light)); break;
        }
    }

}
