#ifndef VOX_SCENE_H
#define VOX_SCENE_H

#include "Object.h"

namespace renderbox {

    class Scene : public Object {
    protected:

        glm::vec3 ambientColor = glm::vec3(0.0f);

    public:

        glm::vec3 getAmbientColor() const;

        void setAmbientColor(glm::vec3 ambientColor);

    };

}


#endif //VOX_SCENE_H
