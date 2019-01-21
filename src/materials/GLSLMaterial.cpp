#include "GLSLMaterial.h"


namespace renderbox {

    GLSLMaterial::GLSLMaterial(const std::string &vertexShaderSource,
                               const std::string &fragmentShaderSource)
        : vertexShaderSource(vertexShaderSource), fragmentShaderSource(fragmentShaderSource) {}

    char const *GLSLMaterial::getOpenGLVertexShaderSource(Geometry *geometry) const {
        return vertexShaderSource.c_str();
    };

    char const *GLSLMaterial::getOpenGLFragmentShaderSource(Geometry *geometry) const {
        return fragmentShaderSource.c_str();
    };

}
