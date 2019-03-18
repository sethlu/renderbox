#include "OpenGLMaterialProperties.h"


namespace renderbox {

    OpenGLTexture *OpenGLMaterialProperties::getTexture(int index, bool *blankTexture) {

        auto result = textures.find(index);
        if (result != textures.end()) {
            if (blankTexture) *blankTexture = false;
            return result->second.get();
        }

        auto texture = new OpenGLTexture();
        textures.insert(std::make_pair(index, std::unique_ptr<OpenGLTexture>(texture)));

        if (blankTexture) *blankTexture = true;
        return texture;

    }

}
