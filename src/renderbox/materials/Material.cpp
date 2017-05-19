#include "Material.h"


namespace renderbox {

    int Material::count = 0;

    Material::Material() {

    }

    int Material::getMaterialID() {
        return id;
    }

}
