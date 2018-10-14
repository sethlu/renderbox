#ifndef RENDERBOX_OPENGLRENDERERPROPERTIES_H
#define RENDERBOX_OPENGLRENDERERPROPERTIES_H


#include <unordered_map>
#include <string>

#include "Mesh.h"
#include "OpenGL.h"
#include "OpenGLObjectProperties.h"

namespace renderbox {

    class OpenGLRendererProperties {
    public:

        OpenGLProgram *getProgram(Material *material, bool forceRecompile = false);

        OpenGLObjectProperties *getObjectProperties(Object *object, bool *blankObjectProperties = nullptr);

        unsigned numPointLights = 0;

        unsigned lastNumPointLights = 0;

    private:

        static std::unordered_map<int, std::unique_ptr<OpenGLProgram>> programs;

        std::unordered_map<int, std::unique_ptr<OpenGLObjectProperties>> objectProperties;

    };

}


#endif //RENDERBOX_OPENGLRENDERERPROPERTIES_H
