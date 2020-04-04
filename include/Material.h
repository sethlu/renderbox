#ifndef RENDERBOX_MATERIAL_H
#define RENDERBOX_MATERIAL_H


#include <string>

#include "Vector.h"
#include "Texture.h"
#include "VersionTrackedObject.h"
#include "OpenGLProgram.h"
#include "Geometry.h"
#include "MeshGeometry.h"


namespace renderbox {

    enum MATERIAL_TYPE {
        POINT_MATERIAL      = 0x01,
        LINE_MATERIAL       = 0x02,
        MESH_MATERIAL       = 0x04,
        AMBIENT_MATERIAL    = 0x08,
        DIFFUSE_MATERIAL    = 0x10,
    };

    // Objects using materials with texture maps must present UV coordinates

    class Material : public VersionTrackedObject {
    public:

        typedef unsigned int id_type;

        virtual ~Material() = default;

        inline id_type getMaterialId() const {
            return materialId;
        }

        virtual unsigned int getMaterialType() const = 0;

        bool isAmbientMaterial() const {
            return (getMaterialType() & AMBIENT_MATERIAL) != 0;
        }

        bool isDiffuseMaterial() const {
            return (getMaterialType() & DIFFUSE_MATERIAL) != 0;
        }

        bool supportsMeshGeometry() const {
            return (getMaterialType() & MESH_MATERIAL) != 0;
        }

        bool supportsGeometry(Geometry *geometry) const {
            return supportsMeshGeometry() && dynamic_cast<MeshGeometry *>(geometry);
        }

        bool supportsGeometry(std::shared_ptr<Geometry> const &geometry) const {
            return supportsGeometry(geometry.get());
        }

    protected:

        static unsigned int count;

        id_type materialId = ++count;

    };

    class AmbientMaterial {
    public:

        explicit AmbientMaterial(vec3 const &ambientColor) : _ambientColor(ambientColor) {};

        vec3 getAmbientColor() const { return _ambientColor; }

        void setAmbientColor(vec3 const &ambientColor) { _ambientColor = ambientColor; }

        std::shared_ptr<Texture> getAmbientMap() const { return _ambientMap; }

        void setAmbientMap(std::shared_ptr<Texture> const &ambientMap) { _ambientMap = ambientMap; }

    protected:

        vec3 _ambientColor;

        std::shared_ptr<Texture> _ambientMap;

    };

    class DiffuseMaterial {
    public:

        explicit DiffuseMaterial(vec3 const &diffuseColor) : _diffuseColor(diffuseColor) {};

        vec3 getDiffuseColor() const { return _diffuseColor; }

        void setDiffuseColor(vec3 const &diffuseColor) { _diffuseColor = diffuseColor; }

        std::shared_ptr<Texture> getDiffuseMap() const { return _diffuseMap; }

        void setDiffuseMap(std::shared_ptr<Texture> const &diffuseMap) { _diffuseMap = diffuseMap; }

    protected:

        vec3 _diffuseColor;

        std::shared_ptr<Texture> _diffuseMap;

    };

    class OpenGLMaterial {
    public:

        virtual std::string getOpenGLVertexShaderTag(Geometry const *geometry) const = 0;

        virtual char const *getOpenGLVertexShaderSource(Geometry const *geometry) const = 0;

        virtual std::string getOpenGLFragmentShaderTag(Geometry const *geometry) const = 0;

        virtual char const *getOpenGLFragmentShaderSource(Geometry const *geometry) const = 0;

    };

    class MetalMaterial {
    public:

        virtual std::string getMetalVertexFunctionName(Geometry *geometry) const = 0;

        virtual std::string getMetalFragmentFunctionName(Geometry *geometry) const = 0;

    };

}

#endif //RENDERBOX_MATERIAL_H
