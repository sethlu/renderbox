#include "OpenGLRenderList.h"


namespace renderbox {

    void OpenGLRenderList::addObject(OpenGLProgram const *program, Object *object) {
        auto result = objects.find(program);
        if (result != objects.end()) {
            result->second.insert(result->second.end(), object);
        } else {
            std::vector<Object *> objectList;
            objectList.insert(objectList.end(), object);
            objects.insert(std::make_pair(program, objectList));
        }
    }

    void OpenGLRenderList::addLight(Light *light) {
        switch (light->getLightType()) {
            default: break;
            case POINT_LIGHT: pointLights.emplace_back(static_cast<PointLight *>(light)); break;
        }
    }

}