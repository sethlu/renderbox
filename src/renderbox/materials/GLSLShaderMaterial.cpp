#include "GLSLShaderMaterial.h"


namespace renderbox {

    GLSLShaderMaterial::GLSLShaderMaterial(const char *vertexShaderSource, const char *fragmentShaderSource) {
        this->vertexShaderSource = vertexShaderSource;
        this->fragmentShaderSource = fragmentShaderSource;
    }

    GLSLShaderMaterial::GLSLShaderMaterial(std::string vertexShaderSource, std::string fragmentShaderSource) : GLSLShaderMaterial(copyString(vertexShaderSource), copyString(fragmentShaderSource)) {

    }

    inline MATERIAL GLSLShaderMaterial::getMaterialType() {
        return GLSL_SHADER_MATERIAL;
    }

    const char *GLSLShaderMaterial::getVertexShaderSource() {
        return vertexShaderSource;
    }

    const char *GLSLShaderMaterial::getFragmentShaderSource() {
        return fragmentShaderSource;
    }

}