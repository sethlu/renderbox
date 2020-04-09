#ifndef RENDERBOX_SHADERMATERIAL_H
#define RENDERBOX_SHADERMATERIAL_H


#include <string>

#include "Material.h"

namespace renderbox {

    class GLSLMaterial : public Material, public OpenGLMaterial {
    public:

        GLSLMaterial(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        std::string getOpenGLVertexShaderTag(Geometry const *geometry) const override;

        char const *getOpenGLVertexShaderSource(Geometry const *geometry) const override;

        std::string getOpenGLFragmentShaderTag(Geometry const *geometry) const override;

        char const *getOpenGLFragmentShaderSource(Geometry const *geometry) const override;

        unsigned int getMaterialType() const override {
            return MESH_MATERIAL;
        }

    private:

        std::string vertexShaderSource;

        std::string fragmentShaderSource;

    };

}


#endif //RENDERBOX_SHADERMATERIAL_H
