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
        int id = ++count;
    public:
        Material();
        int getMaterialID();
        virtual MATERIAL getMaterialType() = 0;
    };

    char *copyString(std::string src);

}

#endif //VOX_MATERIAL_H
