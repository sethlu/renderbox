#ifndef VOX_SHADERMATERIAL_H
#define VOX_SHADERMATERIAL_H


#include <string>
#include "Material.h"

namespace renderbox {

    class GLSLShaderMaterial : public Material {

        const char *vertexShaderSource;

        const char *fragmentShaderSource;

    public:

        GLSLShaderMaterial(const char *vertexShaderSource, const char *fragmentShaderSource);

        GLSLShaderMaterial(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        const char *getVertexShaderSource() const;

        const char *getFragmentShaderSource() const;

        MATERIAL getMaterialType() const override;

    };

}


#endif //VOX_SHADERMATERIAL_H
