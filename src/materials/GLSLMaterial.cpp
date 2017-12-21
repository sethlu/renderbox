#include "GLSLMaterial.h"


namespace renderbox {

    GLSLMaterial::GLSLMaterial(const std::string &vertexShaderSource,
                               const std::string &fragmentShaderSource)
        : vertexShaderSource(vertexShaderSource), fragmentShaderSource(fragmentShaderSource) {

    }

    const char *GLSLMaterial::getVertexShaderSource() const {
        return vertexShaderSource.c_str();
    };

    const char *GLSLMaterial::getFragmentShaderSource() const {
        return fragmentShaderSource.c_str();
    };

    MATERIAL_TYPE GLSLMaterial::getMaterialType() const {
        return GLSL_MATERIAL;
    }

}
