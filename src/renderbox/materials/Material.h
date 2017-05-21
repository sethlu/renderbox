#ifndef PARTICLESIM_MATERIAL_H
#define PARTICLESIM_MATERIAL_H


namespace renderbox {

    enum MATERIAL {
        GLSL_SHADER_MATERIAL,
    };

    class Material {
    private:
        static int count;
        int id = ++count;
    public:
        Material();
        int getMaterialID();
        virtual MATERIAL getMaterialType() = 0;
    };

}

#endif //PARTICLESIM_MATERIAL_H
