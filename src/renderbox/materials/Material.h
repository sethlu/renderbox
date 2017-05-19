#ifndef PARTICLESIM_MATERIAL_H
#define PARTICLESIM_MATERIAL_H


namespace renderbox {

    class Material {
        static int count;
        int id = ++count;
    public:
        Material();
        int getMaterialID();
    };

}

#endif //PARTICLESIM_MATERIAL_H
