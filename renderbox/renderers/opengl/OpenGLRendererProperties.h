#ifndef VOX_OPENGLRENDERERPROPERTIES_H
#define VOX_OPENGLRENDERERPROPERTIES_H


#include "../../platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <unordered_map>
#include <string>
#include "../../objects/Mesh.h"
#include "OpenGLObjectProperties.h"

namespace renderbox {

    class OpenGLRendererProperties {
        static std::unordered_map<int, OpenGLProgram *> programs;
        std::unordered_map<int, OpenGLObjectProperties *> objectProperties;
    public:
        static OpenGLProgram *getProgram(Material *material);
        OpenGLObjectProperties *getObjectProperties(Object *object);
    };

}


#endif //VOX_OPENGLRENDERERPROPERTIES_H
