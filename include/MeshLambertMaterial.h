#ifndef RENDERBOX_MESHLAMBERTMATERIAL_H
#define RENDERBOX_MESHLAMBERTMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshLambertMaterial : public Material, public AmbientMaterial, public DiffuseMaterial, public OpenGLMaterial, public MetalMaterial {
    public:

        explicit MeshLambertMaterial(vec3 diffuseColor = vec3(1.0f))
                : MeshLambertMaterial(vec3(1.0f), diffuseColor) {

        };

        MeshLambertMaterial(vec3 ambientColor,
                            vec3 diffuseColor)
                : AmbientMaterial(ambientColor), DiffuseMaterial(diffuseColor) {

        };

        unsigned int getMaterialType() const override {
            return MESH_MATERIAL | AMBIENT_MATERIAL | DIFFUSE_MATERIAL;
        };

        std::string getOpenGLVertexShaderTag(Geometry const *geometry) const override;

        char const *getOpenGLVertexShaderSource(Geometry const *geometry) const override;

        std::string getOpenGLFragmentShaderTag(Geometry const *geometry) const override;

        char const *getOpenGLFragmentShaderSource(Geometry const *geometry) const override;

        std::string getMetalVertexFunctionName(Geometry *geometry) const override;

        std::string getMetalFragmentFunctionName(Geometry *geometry) const override;

    };

}


#endif //RENDERBOX_MESHLAMBERTMATERIAL_H
