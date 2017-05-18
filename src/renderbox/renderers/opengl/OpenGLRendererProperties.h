#include <GL/glew.h>
#include <map>
#include <string>
#include "../../objects/Mesh.h"
#include "OpenGLObjectProperties.h"


#ifndef PARTICLESIM_OPENGLPROPERTIES_H
#define PARTICLESIM_OPENGLPROPERTIES_H


namespace renderbox {

    class OpenGLRendererProperties {
        std::map<int, OpenGLObjectProperties *> objectProperties;
    public:
        OpenGLObjectProperties *getObjectProperties(Object *object);
    };

}


#endif //PARTICLESIM_OPENGLPROPERTIES_H
