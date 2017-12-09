#include "GLSLMaterial.h"


namespace renderbox {

    GLSLMaterial::GLSLMaterial(const char *vertexShaderSource, const char *fragmentShaderSource)
        : vertexShaderSource(vertexShaderSource)
        , fragmentShaderSource(fragmentShaderSource) {

    }

    GLSLMaterial::GLSLMaterial(const std::string &vertexShaderSource,
                                           const std::string &fragmentShaderSource)
        : GLSLMaterial(copyString(vertexShaderSource), copyString(fragmentShaderSource)) {

    }

    const char *GLSLMaterial::getVertexShaderSource() const {
        return vertexShaderSource;
    };

    const char *GLSLMaterial::getFragmentShaderSource() const {
        return fragmentShaderSource;
    };

    MATERIAL GLSLMaterial::getMaterialType() const {
        return GLSL_MATERIAL;
    };

}
