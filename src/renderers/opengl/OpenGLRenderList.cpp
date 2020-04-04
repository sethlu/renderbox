#include "OpenGLRenderList.h"


namespace renderbox {

    void OpenGLRenderList::addObject(OpenGLProgram const *program, Object const *object) {
        auto result = objects.find(program);
        if (result != objects.end()) {
            result->second.insert(result->second.end(), object);
        } else {
            objects.insert(std::make_pair(program, std::vector<Object const *>{object}));
        }
    }

    void OpenGLRenderList::addLight(Light *light) {
        switch (light->getLightType()) {
            default: break;
            case POINT_LIGHT: pointLights.emplace_back(static_cast<PointLight *>(light)); break;
        }
    }

}