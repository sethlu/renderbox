#ifndef RENDERBOX_MESHBASICMATERIAL_H
#define RENDERBOX_MESHBASICMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material, public DiffuseMaterial, public OpenGLMaterial, public MetalMaterial {
    public:

        explicit MeshBasicMaterial(vec3 diffuseColor = vec3(1.0f))
                : DiffuseMaterial(diffuseColor) {}

        unsigned int getMaterialType() const override {
            return MESH_MATERIAL | DIFFUSE_MATERIAL;
        }

        std::string getOpenGLVertexShaderTag(Geometry const *geometry) const override;

        char const *getOpenGLVertexShaderSource(Geometry const *geometry) const override;

        std::string getOpenGLFragmentShaderTag(Geometry const *geometry) const override;

        char const *getOpenGLFragmentShaderSource(Geometry const *geometry) const override;

        std::string getMetalVertexFunctionName(Geometry const *geometry) const override;

        std::string getMetalFragmentFunctionName(Geometry const *geometry) const override;

    };

}


#endif //RENDERBOX_MESHBASICMATERIAL_H
