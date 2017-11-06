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

        static int count;

        int materialId = ++count;

    public:

        Material();

        int getMaterialId();

        virtual MATERIAL getMaterialType() const = 0;

    };

    char *copyString(const std::string &src);

}

#endif //VOX_MATERIAL_H
