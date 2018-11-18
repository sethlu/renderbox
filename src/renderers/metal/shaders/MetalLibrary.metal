#include <metal_stdlib>

using namespace metal;

#define SCREEN_GAMMA 2.2
#define NUM_POINT_LIGHTS 4

typedef struct {
    float3 position;
    float3 color;
} PointLight;

typedef struct {
    float4x4 worldProjectionMatrix;
    float4x4 worldMatrix;
    float3x3 worldNormalMatrix;
    float3 sceneAmbientColor;
    float3 materialAmbientColor;
    float3 materialDiffuseColor;
    uint numActivePointLights;
    PointLight pointLights[NUM_POINT_LIGHTS];
} Uniforms;

typedef struct {
    float4 position [[position]];
    float2 uv;
    float3 ambientColor;
    float3 diffuseColor;
} LambertVertexOut;

float4 gammaToLinear(float4 value, float gamma) {
    return pow(value, float4(gamma));
}

float4 linearToGamma(float4 value, float gamma) {
    return pow(value, float4(1.0 / gamma));
}

float3 dehomogenize(float4 vector) {
    return vector.xyz / vector.w;
}

constexpr sampler textureSampler(mag_filter::linear, min_filter::linear);

// Vertex

#include "_lambert_vert.metal"
#define RB_VERTEX_UV
#include "_lambert_vert.metal"
#undef RB_VERTEX_UV

// Fragment

#include "_lambert_frag.metal"
#define RB_MATERIAL_AMBIENT_MAP
#include "_lambert_frag.metal"
#define RB_MATERIAL_DIFFUSE_MAP
#include "_lambert_frag.metal"
#undef RB_MATERIAL_AMBIENT_MAP
#include "_lambert_frag.metal"
#undef RB_MATERIAL_DIFFUSE_MAP
