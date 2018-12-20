#ifndef RENDERBOX_MATERIAL_H
#define RENDERBOX_MATERIAL_H


#include <string>

#include "Vector.h"
#include "Texture.h"
#include "VersionTrackedObject.h"

namespace renderbox {

    enum MATERIAL_TYPE {
        MESH_BASIC_MATERIAL   = 0x01,
        MESH_LAMBERT_MATERIAL = 0x02,
        GLSL_MATERIAL         = 0x04,
        AMBIENT_MATERIAL      = MESH_LAMBERT_MATERIAL,
        DIFFUSE_MATERIAL      = MESH_BASIC_MATERIAL | MESH_LAMBERT_MATERIAL,
    };

    // Objects using materials with texture maps must present UV coordinates

    class Material : public VersionTrackedObject {
    public:

        Material() = default;

        virtual ~Material();

        int getMaterialId();

        virtual MATERIAL_TYPE getMaterialType() const = 0;

        bool isAmbientMaterial() const {
            return (getMaterialType() & AMBIENT_MATERIAL) != 0;
        }

        bool isDiffuseMaterial() const {
            return (getMaterialType() & DIFFUSE_MATERIAL) != 0;
        }

    protected:

        static int count;

        int materialId = ++count;

    };

    class AmbientMaterial {
    public:

        virtual ~AmbientMaterial();

        explicit AmbientMaterial(vec3 ambientColor) : ambientColor(ambientColor) {};

        vec3 getAmbientColor() const { return ambientColor; }

        void setAmbientColor(vec3 &ambientColor_) { ambientColor = ambientColor_; }

        std::shared_ptr<Texture> getAmbientMap() const { return ambientMap; }

        void setAmbientMap(std::shared_ptr<Texture> ambientMap_) { ambientMap = ambientMap_; }

    protected:

        vec3 ambientColor;

        std::shared_ptr<Texture> ambientMap;

    };

    class DiffuseMaterial {
    public:

        virtual ~DiffuseMaterial();

        explicit DiffuseMaterial(vec3 diffuseColor) : diffuseColor(diffuseColor) {};

        vec3 getDiffuseColor() const { return diffuseColor; }

        void setDiffuseColor(vec3 &diffuseColor_) { diffuseColor = diffuseColor_; }

        std::shared_ptr<Texture> getDiffuseMap() const { return diffuseMap; }

        void setDiffuseMap(std::shared_ptr<Texture> diffuseMap_) { diffuseMap = diffuseMap_; }

    protected:

        vec3 diffuseColor;

        std::shared_ptr<Texture> diffuseMap;

    };

}

#endif //RENDERBOX_MATERIAL_H
