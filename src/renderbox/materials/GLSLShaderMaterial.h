#ifndef PARTICLESIM_SHADERMATERIAL_H
#define PARTICLESIM_SHADERMATERIAL_H


#include <string>
#include "Material.h"

namespace renderbox {

    class GLSLShaderMaterial : public Material {
        const char *vertexShaderSource;
        const char *fragmentShaderSource;
    public:
        GLSLShaderMaterial(const char *vertexShaderSource, const char *fragmentShaderSource);
        GLSLShaderMaterial(std::string vertexShaderSource, std::string fragmentShaderSource);
        const char *getVertexShaderSource();
        const char *getFragmentShaderSource();
        MATERIAL getMaterialType();
    };

}


#endif //PARTICLESIM_SHADERMATERIAL_H
