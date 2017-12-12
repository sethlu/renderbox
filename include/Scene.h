#ifndef VOX_SCENE_H
#define VOX_SCENE_H

#include "Object.h"

namespace renderbox {

    class Scene : public Object {
    public:

        glm::vec3 getAmbientColor() const;

        void setAmbientColor(glm::vec3 ambientColor);

    protected:

        glm::vec3 ambientColor = glm::vec3(0.0f);

    };

}


#endif //VOX_SCENE_H
