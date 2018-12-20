#ifdef RB_MATERIAL_DIFFUSE_MAP
fragment float4 mesh_basic_frag_diffusemap(
#else
fragment float4 mesh_basic_frag(
#endif
                             BasicVertexOut in [[stage_in]]
#ifdef RB_MATERIAL_DIFFUSE_MAP
                             , texture2d<float> diffuseMap [[texture(1)]]
#endif
                             ) {

#ifdef RB_MATERIAL_DIFFUSE_MAP
    float4 diffuseMapValue = diffuseMap.sample(textureSampler, in.uv);
#endif

    float4 diffuseLinear = float4(in.diffuseColor, 1);
#ifdef RB_MATERIAL_DIFFUSE_MAP
    diffuseLinear *= gammaToLinear(diffuseMapValue, SCREEN_GAMMA);
#endif

    float4 colorLinear = diffuseLinear;

    return linearToGamma(colorLinear, SCREEN_GAMMA);

}
