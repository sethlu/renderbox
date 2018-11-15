#include <metal_stdlib>
using namespace metal;

typedef struct {
    float4x4 worldProjectionMatrix;
} Uniforms;

typedef struct {
    packed_float3 position;
} VertexIn;

typedef struct {
    float4 position [[position]];
    float4 color;
} VertexOut;

vertex VertexOut vertex_function(device VertexIn *vertices [[buffer(0)]],
                                 constant Uniforms &uniforms [[buffer(1)]],
                                 uint vid [[vertex_id]]) {
    VertexOut out;
    out.position = uniforms.worldProjectionMatrix * float4(vertices[vid].position, 1);
    out.color = float4(1, 1, 1, 1);
    return out;
}

fragment float4 fragment_function(VertexOut in [[stage_in]]) {
    return in.color;
}
