#include "Scene.h"


namespace renderbox {

    Scene::Scene() : ambientColor(vec3(0)) {

    }

    glm::vec3 Scene::getAmbientColor() const {
        return ambientColor;
    }

    void Scene::setAmbientColor(const glm::vec3 &ambientColor_) {
        ambientColor = ambientColor_;
    }

}
