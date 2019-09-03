#ifdef RB_VERTEX_SKIN
#ifdef RB_VERTEX_UV
vertex LambertVertexOut mesh_lambert_vert_skin_uv(
#else
vertex LambertVertexOut mesh_lambert_vert_skin(
#endif // RB_VERTEX_UV
#else
#ifdef RB_VERTEX_UV
vertex LambertVertexOut mesh_lambert_vert_uv(
#else
vertex LambertVertexOut mesh_lambert_vert(
#endif // RB_VERTEX_UV
#endif // RB_VERTEX_SKIN
                                          device packed_float3 *vertices [[buffer(0)]],
#ifdef RB_VERTEX_UV
                                          device packed_float2 *uvs [[buffer(1)]],
#endif
                                          device packed_float3 *normals [[buffer(2)]],
                                          constant Uniforms &uniforms [[buffer(3)]],
#ifdef RB_VERTEX_SKIN
                                          constant uint4 *skinIndices [[buffer(4)]],
                                          constant float4 *skinWeights [[buffer(5)]],
#endif // RB_VERTEX_SKIN
                                          uint vid [[vertex_id]]) {
    LambertVertexOut out;

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
    float3 vertexWorldPosition = dehomogenize(uniforms.worldMatrix * localPosition);

    // Normal

    auto localNormal = float3(normals[vid]);

#ifdef RB_VERTEX_SKIN
    localNormal = float3x3(float3(boneMatrix[0]),
                           float3(boneMatrix[1]),
                           float3(boneMatrix[2])) * localNormal;
#endif // RB_VERTEX_SKIN

    float3 vertexWorldNormal = uniforms.worldNormalMatrix * localNormal;

    // UV

#ifdef RB_VERTEX_UV
    out.uv = uvs[vid];
#endif

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
