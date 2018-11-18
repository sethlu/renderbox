
#ifdef RB_MATERIAL_AMBIENT_MAP
#ifdef RB_MATERIAL_DIFFUSE_MAP
fragment float4 lambert_frag_ambientmap_diffusemap(
#else
fragment float4 lambert_frag_ambientmap(
#endif
#else
#ifdef RB_MATERIAL_DIFFUSE_MAP
fragment float4 lambert_frag_diffusemap(
#else
fragment float4 lambert_frag(
#endif
#endif
                             LambertVertexOut in [[stage_in]]
#ifdef RB_MATERIAL_AMBIENT_MAP
                             , texture2d<float> ambientMap [[texture(0)]]
#endif
#ifdef RB_MATERIAL_DIFFUSE_MAP
                             , texture2d<float> diffuseMap [[texture(1)]]
#endif
                             ) {

#ifdef RB_MATERIAL_AMBIENT_MAP
    float4 ambientMapValue = ambientMap.sample(textureSampler, in.uv);
#endif

#ifdef RB_MATERIAL_DIFFUSE_MAP
    float4 diffuseMapValue = diffuseMap.sample(textureSampler, in.uv);
#endif

    float4 ambientLinear = float4(in.ambientColor, 1);
#ifdef RB_MATERIAL_AMBIENT_MAP
    ambientLinear *= gammaToLinear(ambientMapValue, SCREEN_GAMMA);
#endif

    float4 diffuseLinear = float4(in.diffuseColor, 1);
#ifdef RB_MATERIAL_DIFFUSE_MAP
    diffuseLinear *= gammaToLinear(diffuseMapValue, SCREEN_GAMMA);
#endif

    float4 colorLinear = ambientLinear + diffuseLinear;

    return linearToGamma(colorLinear, SCREEN_GAMMA);

}
