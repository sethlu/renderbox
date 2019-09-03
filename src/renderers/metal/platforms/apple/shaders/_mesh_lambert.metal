typedef struct {
    float4 position [[position]];
    float2 uv;
    float3 ambientColor;
    float3 diffuseColor;
} LambertVertexOut;

// Vertex

#include "_mesh_lambert_vert.metal"
#define RB_VERTEX_UV
#include "_mesh_lambert_vert.metal"
#define RB_VERTEX_SKIN
#include "_mesh_lambert_vert.metal"
#undef RB_VERTEX_UV
#include "_mesh_lambert_vert.metal"
#undef RB_VERTEX_SKIN

// Fragment

#include "_mesh_lambert_frag.metal"
#define RB_MATERIAL_AMBIENT_MAP
#include "_mesh_lambert_frag.metal"
#define RB_MATERIAL_DIFFUSE_MAP
#include "_mesh_lambert_frag.metal"
#undef RB_MATERIAL_AMBIENT_MAP
#include "_mesh_lambert_frag.metal"
#undef RB_MATERIAL_DIFFUSE_MAP
