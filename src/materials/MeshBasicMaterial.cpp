#include "MeshBasicMaterial.h"

#include "MeshGeometry.h"


static char const * const GLSL_VERT_120 =
#include "opengl/shaders/120/mesh_basic_vert.glsl"

static char const * const GLSL_VERT_330 =
#include "opengl/shaders/330/mesh_basic_vert.glsl"

static char const * const GLSL_VERT_300ES =
#include "opengl/shaders/300es/mesh_basic_vert.glsl"

static char const * const GLSL_FRAG_120 =
#include "opengl/shaders/120/mesh_basic_frag.glsl"

static char const * const GLSL_FRAG_330 =
#include "opengl/shaders/330/mesh_basic_frag.glsl"

static char const * const GLSL_FRAG_300ES =
#include "opengl/shaders/300es/mesh_basic_frag.glsl"

#define _RETURN_SHADER(TYPE, VERSION) return GLSL_ ## TYPE ## _ ## VERSION
#define RETURN_SHADER(...) _RETURN_SHADER(__VA_ARGS__)


namespace renderbox {

    std::string MeshBasicMaterial::getOpenGLVertexShaderTag(Geometry const *geometry) const {
        return "basic";
    }

    char const *MeshBasicMaterial::getOpenGLVertexShaderSource(Geometry const *geometry) const {
        RETURN_SHADER(VERT, RENDERBOX_OPENGL_GLSL_VERSION);
    }

    std::string MeshBasicMaterial::getOpenGLFragmentShaderTag(Geometry const *geometry) const {
        return "basic";
    }

    char const *MeshBasicMaterial::getOpenGLFragmentShaderSource(Geometry const *geometry) const {
        RETURN_SHADER(FRAG, RENDERBOX_OPENGL_GLSL_VERSION);
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
