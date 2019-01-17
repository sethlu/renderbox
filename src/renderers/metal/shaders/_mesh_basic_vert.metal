#ifdef RB_VERTEX_SKIN
#ifdef RB_VERTEX_UV
vertex BasicVertexOut mesh_basic_vert_skin_uv(
#else
vertex BasicVertexOut mesh_basic_vert_skin(
#endif // RB_VERTEX_UV
#else
#ifdef RB_VERTEX_UV
vertex BasicVertexOut mesh_basic_vert_uv(
#else
vertex BasicVertexOut mesh_basic_vert(
#endif // RB_VERTEX_UV
#endif // RB_VERTEX_SKIN
                                      device packed_float3 *vertices [[buffer(0)]],
#ifdef RB_VERTEX_UV
                                      device packed_float2 *uvs [[buffer(1)]],
#endif // RB_VERTEX_UV
                                      device packed_float3 *normals [[buffer(2)]],
                                      constant Uniforms &uniforms [[buffer(3)]],
#ifdef RB_VERTEX_SKIN
                                      constant uint4 *skinIndices [[buffer(4)]],
                                      constant float4 *skinWeights [[buffer(5)]],
#endif // RB_VERTEX_SKIN
                                      uint vid [[vertex_id]]) {
    BasicVertexOut out;

    // Position

    auto localPosition = float4(vertices[vid], 1);

#ifdef RB_VERTEX_SKIN
    float4x4 boneMatrix = uniforms.boneMatrics[skinIndices[vid][0]] * skinWeights[vid][0];
    boneMatrix += uniforms.boneMatrics[skinIndices[vid][1]] * skinWeights[vid][1];
    boneMatrix += uniforms.boneMatrics[skinIndices[vid][2]] * skinWeights[vid][2];
    boneMatrix += uniforms.boneMatrics[skinIndices[vid][3]] * skinWeights[vid][3];
    localPosition = boneMatrix * localPosition;
#endif // RB_VERTEX_SKIN

    out.position = uniforms.worldProjectionMatrix * localPosition;

    // UV

#ifdef RB_VERTEX_UV
    out.uv = uvs[vid];
#endif

    // Lighting

    // Diffuse

    out.diffuseColor = uniforms.materialDiffuseColor;

    return out;
}
