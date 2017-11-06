#include "GLSLShaderMaterial.h"


namespace renderbox {

    GLSLShaderMaterial::GLSLShaderMaterial(const char *vertexShaderSource, const char *fragmentShaderSource)
        : vertexShaderSource(vertexShaderSource)
        , fragmentShaderSource(fragmentShaderSource) {

    }

    GLSLShaderMaterial::GLSLShaderMaterial(const std::string &vertexShaderSource,
                                           const std::string &fragmentShaderSource)
        : GLSLShaderMaterial(copyString(vertexShaderSource), copyString(fragmentShaderSource)) {

    }

    const char *GLSLShaderMaterial::getVertexShaderSource() const {
        return vertexShaderSource;
    };

    const char *GLSLShaderMaterial::getFragmentShaderSource() const {
        return fragmentShaderSource;
    };

    MATERIAL GLSLShaderMaterial::getMaterialType() const {
        return GLSL_SHADER_MATERIAL;
    };

}
