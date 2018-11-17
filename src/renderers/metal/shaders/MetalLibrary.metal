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
    float3 color;
} LambertVertexOut;

float3 dehomogenize(float4 vector) {
    return vector.xyz / vector.w;
}

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

    // Lighting

    // Ambient

    float3 vertexAmbientColor = uniforms.sceneAmbientColor * uniforms.materialAmbientColor;

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

    vertexDiffuseColor *= uniforms.materialDiffuseColor;

    out.color = vertexAmbientColor + vertexDiffuseColor;

    return out;
}

fragment float4 lambert_frag(LambertVertexOut in [[stage_in]]) {
    return float4(pow(in.color, float3(1.0 / SCREEN_GAMMA)), 1);
}
