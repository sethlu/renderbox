#include "Scene.h"


namespace renderbox {

    glm::vec3 Scene::getAmbientColor() {
        return ambientColor;
    }

    void Scene::setAmbientColor(glm::vec3 ambientColor) {
        this->ambientColor = ambientColor;
    }

}