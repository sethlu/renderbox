#include "MeshLambertMaterial.h"

#include "MeshGeometry.h"


namespace renderbox {

    char const *MeshLambertMaterial::getOpenGLVertexShaderSource(Geometry *geometry) const {
        return
#if defined(RENDERBOX_OS_MACOS)
#include "opengl/shaders/mesh_lambert_vert.glsl"
#elif defined(RENDERBOX_OS_IPHONEOS)
#include "opengl/shaders/es/mesh_lambert_vert.glsl"
#endif
                ;
    }

    char const *MeshLambertMaterial::getOpenGLFragmentShaderSource(Geometry *geometry) const {
        return
#if defined(RENDERBOX_OS_MACOS)
#include "opengl/shaders/mesh_lambert_frag.glsl"
#elif defined(RENDERBOX_OS_IPHONEOS)
#include "opengl/shaders/es/mesh_lambert_frag.glsl"
#endif
                ;
    }

    std::string MeshLambertMaterial::getMetalVertexFunctionName(Geometry *geometry) const {
        auto const &meshGeometry = dynamic_cast<MeshGeometry *>(geometry);

        std::string vertexFunctionName = "mesh_lambert_vert";

        if (meshGeometry &&
            !meshGeometry->skinIndices.empty() &&
            !meshGeometry->skinWeights.empty()) {
            vertexFunctionName += "_skin";
        }

        if (_diffuseMap) {
            vertexFunctionName += "_uv";
        }

        return vertexFunctionName;

    }

    std::string MeshLambertMaterial::getMetalFragmentFunctionName(Geometry *geometry) const {

        std::string fragmentFunctionName = "mesh_lambert_frag";

        if (_ambientMap || _diffuseMap) {
            fragmentFunctionName += "_diffusemap";
            if (_ambientMap) fragmentFunctionName += "_ambientmap";
            if (_diffuseMap) fragmentFunctionName += "_diffusemap";
        }

        return fragmentFunctionName;

    }

}
