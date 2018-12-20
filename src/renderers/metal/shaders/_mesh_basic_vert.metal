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
                                     uint vid [[vertex_id]]) {
    BasicVertexOut out;

    // Position

    out.position = uniforms.worldProjectionMatrix * float4(vertices[vid], 1);

    // UV

#ifdef RB_VERTEX_UV
    out.uv = uvs[vid];
#endif

    // Lighting

    // Diffuse

    out.diffuseColor = uniforms.materialDiffuseColor;

    return out;
}
