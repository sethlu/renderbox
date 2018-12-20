#ifdef RB_VERTEX_UV
vertex LambertVertexOut mesh_lambert_vert_uv(
#else
vertex LambertVertexOut mesh_lambert_vert(
#endif
                                     device packed_float3 *vertices [[buffer(0)]],
#ifdef RB_VERTEX_UV
                                     device packed_float2 *uvs [[buffer(1)]],
#endif
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
