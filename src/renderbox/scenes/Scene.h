#ifndef VOX_SCENE_H
#define VOX_SCENE_H

#include "../core/Object.h"

namespace renderbox {

    class Scene : public Object {
        glm::vec3 ambientColor = glm::vec3(0.0f);
    public:
        glm::vec3 getAmbientColor();
        void setAmbientColor(glm::vec3 ambientColor);
    };

}


#endif //VOX_SCENE_H
