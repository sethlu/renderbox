#include "Material.h"


namespace renderbox {

    int Material::count = 0;

    Material::~Material() = default;

    int Material::getMaterialId() {
        return materialId;
    }

    AmbientMaterial::~AmbientMaterial() = default;

    DiffuseMaterial::~DiffuseMaterial() = default;

}
