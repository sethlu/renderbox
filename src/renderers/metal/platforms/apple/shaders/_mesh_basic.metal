typedef struct {
    float4 position [[position]];
    float2 uv;
    float3 diffuseColor;
} BasicVertexOut;

// Vertex

#include "_mesh_basic_vert.metal"
#define RB_VERTEX_UV
#include "_mesh_basic_vert.metal"
#define RB_VERTEX_SKIN
#include "_mesh_basic_vert.metal"
#undef RB_VERTEX_UV
#include "_mesh_basic_vert.metal"
#undef RB_VERTEX_SKIN

// Fragment

#include "_mesh_basic_frag.metal"
#define RB_MATERIAL_DIFFUSE_MAP
#include "_mesh_basic_frag.metal"
#undef RB_MATERIAL_DIFFUSE_MAP
