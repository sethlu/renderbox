#ifdef RB_VERTEX_UV
vertex BasicVertexOut mesh_basic_vert_uv(
#else
vertex BasicVertexOut mesh_basic_vert(
#endif
                                      device packed_float3 *vertices [[buffer(0)]],
#ifdef RB_VERTEX_UV
                                      device packed_float2 *uvs [[buffer(1)]],
#endif
                                      device packed_float3 *normals [[buffer(2)]],
                                      constant Uniforms &uniforms [[buffer(3)]],
                                      constant uint4 *skinIndices [[buffer(4)]],
                                      constant float4 *skinWeights [[buffer(5)]],
                                      uint vid [[vertex_id]]) {
    BasicVertexOut out;

    // Position

    float4x4 boneMatrix = uniforms.boneMatrics[skinIndices[vid][0]] * skinWeights[vid][0];
    boneMatrix += uniforms.boneMatrics[skinIndices[vid][1]] * skinWeights[vid][1];
    boneMatrix += uniforms.boneMatrics[skinIndices[vid][2]] * skinWeights[vid][2];
    boneMatrix += uniforms.boneMatrics[skinIndices[vid][3]] * skinWeights[vid][3];

    out.position = uniforms.worldProjectionMatrix * boneMatrix * float4(vertices[vid], 1);

    // UV

#ifdef RB_VERTEX_UV
    out.uv = uvs[vid];
#endif

    // Lighting

    // Diffuse

    out.diffuseColor = uniforms.materialDiffuseColor;

    return out;
}
