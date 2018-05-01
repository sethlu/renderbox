#ifndef VOX_OPENGLRENDERERPROPERTIES_H
#define VOX_OPENGLRENDERERPROPERTIES_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <unordered_map>
#include <string>
#include "Mesh.h"
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


#endif //VOX_OPENGLRENDERERPROPERTIES_H
