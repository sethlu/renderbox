#ifndef VOX_MATERIAL_H
#define VOX_MATERIAL_H


#include <string>
#include <glm/vec3.hpp>

namespace renderbox {

    enum MATERIAL_TYPE {
        MESH_BASIC_MATERIAL   = 0x01,
        MESH_LAMBERT_MATERIAL = 0x02,
        GLSL_MATERIAL         = 0x04,
        DIFFUSE_MATERIAL      = MESH_BASIC_MATERIAL | MESH_LAMBERT_MATERIAL,
    };

    class Material {
    public:

        Material() = default;

        int getMaterialId();

        virtual MATERIAL_TYPE getMaterialType() const = 0;

        bool isDiffuseMaterial() const {
            return (getMaterialType() & DIFFUSE_MATERIAL) != 0;
        }

    protected:

        static int count;

        int materialId = ++count;

    };

    class DiffuseMaterial {
    public:

        explicit DiffuseMaterial(glm::vec3 diffuseColor) : diffuseColor(diffuseColor) {};

        glm::vec3 getDiffuseColor() const { return diffuseColor; }

        void setDiffuseColor(glm::vec3 &diffuseColor_) { diffuseColor = diffuseColor_; }

    protected:

        glm::vec3 diffuseColor;

    };

    char *copyString(const std::string &src);

}

#endif //VOX_MATERIAL_H
