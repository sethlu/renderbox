#ifndef PARTICLESIM_SCENE_H
#define PARTICLESIM_SCENE_H

#include "../core/Object.h"

namespace renderbox {

    class Scene : public Object {
        glm::vec3 ambientColor = glm::vec3(0.0f);
    public:
        glm::vec3 getAmbientColor();
        void setAmbientColor(glm::vec3 ambientColor);
    };

}


#endif //PARTICLESIM_SCENE_H
