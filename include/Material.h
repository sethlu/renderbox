#ifndef VOX_MATERIAL_H
#define VOX_MATERIAL_H


#include <string>
#include <glm/vec3.hpp>

namespace renderbox {

    enum MATERIAL {
        MESH_BASIC_MATERIAL   = 0x01,
        MESH_LAMBERT_MATERIAL = 0x02,
        GLSL_MATERIAL         = 0x04,
        COLOR_MATERIAL        = MESH_BASIC_MATERIAL | MESH_LAMBERT_MATERIAL,
    };

    class Material {
    public:

        Material() = default;

        int getMaterialId();

        virtual MATERIAL getMaterialType() const = 0;

        bool isColorMaterial() const {
            return (getMaterialType() & COLOR_MATERIAL) != 0;
        }

    protected:

        static int count;

        int materialId = ++count;

    };

    class ColorMaterial {
    public:

        explicit ColorMaterial(glm::vec3 color) : color(color) {};

        glm::vec3 getColor() const { return color; }

    protected:

        glm::vec3 color;

    };

    char *copyString(const std::string &src);

}

#endif //VOX_MATERIAL_H
