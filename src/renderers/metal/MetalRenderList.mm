#include "MetalRenderList.h"


namespace renderbox {

    void MetalRenderList::addObject(Material *material, Object *object) {
        auto result = objects.find(material);
        if (result != objects.end()) {
            result->second.insert(result->second.end(), object);
        } else {
            std::vector<Object *> objectList;
            objectList.insert(objectList.end(), object);
            objects.insert(std::pair<Material *, std::vector<Object *>>(material, objectList));
        }
    }

    void MetalRenderList::addLight(Light *light) {
        switch (light->getLightType()) {
            default: break;
            case POINT_LIGHT: pointLights.emplace_back(static_cast<PointLight *>(light)); break;
        }
    }

}
