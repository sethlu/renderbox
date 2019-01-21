#include "MeshBasicMaterial.h"

#include "MeshGeometry.h"


namespace renderbox {

    char const *MeshBasicMaterial::getOpenGLVertexShaderSource(Geometry *geometry) const {
        return
#if defined(RENDERBOX_OS_MACOS)
#include "opengl/shaders/mesh_basic_vert.glsl"
#elif defined(RENDERBOX_OS_IPHONEOS)
#include "opengl/shaders/es/mesh_basic_vert.glsl"
#endif
                ;
    }

    char const *MeshBasicMaterial::getOpenGLFragmentShaderSource(Geometry *geometry) const {
        return
#if defined(RENDERBOX_OS_MACOS)
#include "opengl/shaders/mesh_basic_frag.glsl"
#elif defined(RENDERBOX_OS_IPHONEOS)
#include "opengl/shaders/es/mesh_basic_frag.glsl"
#endif
                ;
    }

    std::string MeshBasicMaterial::getMetalVertexFunctionName(Geometry *geometry) const {
        auto const &meshGeometry = dynamic_cast<MeshGeometry *>(geometry);

        std::string vertexFunctionName = "mesh_basic_vert";

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

    std::string MeshBasicMaterial::getMetalFragmentFunctionName(Geometry *geometry) const {

        std::string fragmentFunctionName = "mesh_basic_frag";

        if (_diffuseMap) {
            fragmentFunctionName += "_diffusemap";
        }

        return fragmentFunctionName;

    }

}
