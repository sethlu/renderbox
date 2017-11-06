#include "Material.h"


namespace renderbox {

    int Material::count = 0;

    Material::Material() {

    }

    int Material::getMaterialId() {
        return materialId;
    }

    char *copyString(const std::string &src) {
        auto *dst = new char[src.length() + 1];
        strcpy(dst, src.c_str());
        return dst;
    }

}
