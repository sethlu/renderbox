#ifndef RENDERBOX_OPENGLMATERIALPROPERTIES_H
#define RENDERBOX_OPENGLMATERIALPROPERTIES_H


#include <unordered_map>

#include "OpenGLTexture.h"


namespace renderbox {

    class OpenGLMaterialProperties {
    public:

        OpenGLTexture *getTexture(int index, bool *blankTexture = nullptr);

    protected:

        std::unordered_map<int, std::unique_ptr<OpenGLTexture>> textures;

    };

}


#endif //RENDERBOX_OPENGLMATERIALPROPERTIES_H
