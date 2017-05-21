#include "GLSLShaderMaterial.h"


namespace renderbox {

    GLSLShaderMaterial::GLSLShaderMaterial(char *vertexShaderSource, char *fragmentShaderSource) {
        this->vertexShaderSource = vertexShaderSource;
        this->fragmentShaderSource = fragmentShaderSource;
    }

    GLSLShaderMaterial::GLSLShaderMaterial(std::string vertexShaderSource, std::string fragmentShaderSource) {
        this->vertexShaderSource = new char[vertexShaderSource.length() + 1];
        strcpy(this->vertexShaderSource, vertexShaderSource.c_str());
        this->fragmentShaderSource = new char[fragmentShaderSource.length() + 1];
        strcpy(this->fragmentShaderSource, fragmentShaderSource.c_str());
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