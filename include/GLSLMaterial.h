#ifndef RENDERBOX_SHADERMATERIAL_H
#define RENDERBOX_SHADERMATERIAL_H


#include <string>
#include "Material.h"

namespace renderbox {

    class GLSLMaterial : public Material {
    public:

        GLSLMaterial(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

        const char *getVertexShaderSource() const;

        const char *getFragmentShaderSource() const;

        MATERIAL_TYPE getMaterialType() const override;

    private:

        std::string vertexShaderSource;

        std::string fragmentShaderSource;

    };

}


#endif //RENDERBOX_SHADERMATERIAL_H
