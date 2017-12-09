#ifndef VOX_MATERIAL_H
#define VOX_MATERIAL_H


#include <string>

namespace renderbox {

    enum MATERIAL {
        MESH_BASIC_MATERIAL,
        MESH_LAMBERT_MATERIAL,
        GLSL_SHADER_MATERIAL
    };

    class Material {
    public:

        Material() = default;

        int getMaterialId();

        virtual MATERIAL getMaterialType() const = 0;

    protected:

        static int count;

        int materialId = ++count;

    };

    char *copyString(const std::string &src);

}

#endif //VOX_MATERIAL_H
