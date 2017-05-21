#ifndef PARTICLESIM_MATERIAL_H
#define PARTICLESIM_MATERIAL_H


#include <string>

namespace renderbox {

    enum MATERIAL {
        GLSL_SHADER_MATERIAL,
        MESH_BASIC_MATERIAL
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

#endif //PARTICLESIM_MATERIAL_H
