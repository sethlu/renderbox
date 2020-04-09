#include "GLSLMaterial.h"


namespace renderbox {

    GLSLMaterial::GLSLMaterial(const std::string &vertexShaderSource,
                               const std::string &fragmentShaderSource)
        : vertexShaderSource(vertexShaderSource), fragmentShaderSource(fragmentShaderSource) {}

    std::string GLSLMaterial::getOpenGLVertexShaderTag(Geometry const *geometry) const {
        return "glsl-" + std::to_string(getMaterialId());
    }

    char const *GLSLMaterial::getOpenGLVertexShaderSource(Geometry const *geometry) const {
        return vertexShaderSource.c_str();
    }

    std::string GLSLMaterial::getOpenGLFragmentShaderTag(Geometry const *geometry) const {
        return "glsl-" + std::to_string(getMaterialId());
    }

    char const *GLSLMaterial::getOpenGLFragmentShaderSource(Geometry const *geometry) const {
        return fragmentShaderSource.c_str();
    }

}
