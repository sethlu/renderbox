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

        char const *getOpenGLVertexShaderSource(Geometry *geometry) const override;

        char const *getOpenGLFragmentShaderSource(Geometry *geometry) const override;

        std::string getMetalVertexFunctionName(Geometry *geometry) const override;

        std::string getMetalFragmentFunctionName(Geometry *geometry) const override;

    };

}


#endif //RENDERBOX_MESHBASICMATERIAL_H
