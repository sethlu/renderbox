#ifndef VOX_SHADERMATERIAL_H
#define VOX_SHADERMATERIAL_H


#include <string>
#include "Material.h"

namespace renderbox {

    class GLSLMaterial : public Material {

        const char *vertexShaderSource;

        const char *fragmentShaderSource;

    public:

        GLSLMaterial(const char *vertexShaderSource, const char *fragmentShaderSource);

        GLSLMaterial(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        const char *getVertexShaderSource() const;

        const char *getFragmentShaderSource() const;

        MATERIAL_TYPE getMaterialType() const override;

    };

}


#endif //VOX_SHADERMATERIAL_H
