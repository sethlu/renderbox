#ifndef PARTICLESIM_SHADERMATERIAL_H
#define PARTICLESIM_SHADERMATERIAL_H


#include <string>
#include "Material.h"

namespace renderbox {

    class GLSLShaderMaterial : public Material {
        char *vertexShaderSource;
        char *fragmentShaderSource;
    public:
        GLSLShaderMaterial(char *vertexShaderSource, char *fragmentShaderSource);
        GLSLShaderMaterial(std::string vertexShaderSource, std::string fragmentShaderSource);
        char *getVertexShaderSource();
        char *getFragmentShaderSource();
        MATERIAL getMaterialType();
    };

}


#endif //PARTICLESIM_SHADERMATERIAL_H
