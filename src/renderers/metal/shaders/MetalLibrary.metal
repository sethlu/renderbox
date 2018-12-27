#include <metal_stdlib>

using namespace metal;

#define SCREEN_GAMMA 2.2

#define NUM_MAX_POINT_LIGHTS 4
#define NUM_MAX_BONES 8

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
    PointLight pointLights[NUM_MAX_POINT_LIGHTS];
    float4x4 boneMatrics[NUM_MAX_BONES];
} Uniforms;

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

// Basic

#include "_mesh_basic.metal"

// Lambert

#include "_mesh_lambert.metal"
