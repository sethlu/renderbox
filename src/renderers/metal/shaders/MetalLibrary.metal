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

vertex LambertVertexOut lambert_vert(device packed_float3 *vertices [[buffer(0)]],
                                     device packed_float2 *uvs [[buffer(1)]],
                                     device packed_float3 *normals [[buffer(2)]],
                                     constant Uniforms &uniforms [[buffer(3)]],
                                     uint vid [[vertex_id]]) {
    LambertVertexOut out;

    // Position

    out.position = uniforms.worldProjectionMatrix * float4(vertices[vid], 1);
    float3 vertexWorldPosition = dehomogenize(uniforms.worldMatrix * float4(vertices[vid], 1));

    // Normal

    float3 vertexWorldNormal = uniforms.worldNormalMatrix * float3(normals[vid]);

    // UV

    out.uv = uvs[vid];

    // Lighting

    // Ambient

    out.ambientColor = uniforms.sceneAmbientColor * uniforms.materialAmbientColor;

    // Diffuse

    float3 vertexDiffuseColor = float3(0);

    float3 N = vertexWorldNormal;
    for (uint i = 0; i < uniforms.numActivePointLights; i++) {
        float3 lightVector = uniforms.pointLights[i].position - vertexWorldPosition;
        float3 L = normalize(lightVector);
        float lightDistance = length(lightVector);

        vertexDiffuseColor += uniforms.pointLights[i].color *
            max(dot(N, L), 0.0) / (1.0 + (0.25 * lightDistance * lightDistance));
    }

    out.diffuseColor = vertexDiffuseColor * uniforms.materialDiffuseColor;

    return out;
}

fragment float4 lambert_frag(LambertVertexOut in [[stage_in]],
                             texture2d<float> ambientMap [[texture(0)]],
                             texture2d<float> diffuseMap [[texture(1)]]) {

    float4 ambientMapValue = ambientMap.sample(textureSampler, in.uv);
    float4 diffuseMapValue = diffuseMap.sample(textureSampler, in.uv);

    float4 ambientLinear = float4(in.ambientColor, 1) * gammaToLinear(ambientMapValue, SCREEN_GAMMA);
    float4 diffuseLinear = float4(in.diffuseColor, 1) * gammaToLinear(diffuseMapValue, SCREEN_GAMMA);

    float4 colorLinear = ambientLinear + diffuseLinear;

    return linearToGamma(colorLinear, SCREEN_GAMMA);

}
