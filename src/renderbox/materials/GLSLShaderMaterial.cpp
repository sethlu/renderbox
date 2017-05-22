#include "GLSLShaderMaterial.h"


namespace renderbox {

    GLSLShaderMaterial::GLSLShaderMaterial(char *vertexShaderSource, char *fragmentShaderSource) {
        this->vertexShaderSource = vertexShaderSource;
        this->fragmentShaderSource = fragmentShaderSource;
    }

    GLSLShaderMaterial::GLSLShaderMaterial(std::string vertexShaderSource, std::string fragmentShaderSource) : GLSLShaderMaterial(copyString(vertexShaderSource), copyString(fragmentShaderSource)) {

    }

    inline MATERIAL GLSLShaderMaterial::getMaterialType() {
        return GLSL_SHADER_MATERIAL;
    }

    char *GLSLShaderMaterial::getVertexShaderSource() {
        return vertexShaderSource;
    }

    char *GLSLShaderMaterial::getFragmentShaderSource() {
        return fragmentShaderSource;
    }

}