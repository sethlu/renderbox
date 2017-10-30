#include "Material.h"


namespace renderbox {

    int Material::count = 0;

    Material::Material() {

    }

    int Material::getMaterialID() {
        return id;
    }

    char *copyString(std::string src) {
        char *dst = new char[src.length() + 1];
        strcpy(dst, src.c_str());
        return dst;
    }

}
