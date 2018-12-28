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

    void Scene::updateAnimations(float t) {
        for (auto &animationMixer : animationMixers) {
            animationMixer->update(t);
        }
    }

}
