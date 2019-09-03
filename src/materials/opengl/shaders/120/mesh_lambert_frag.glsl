R"(

#version 120

#include <common>

const float screenGamma = 2.2;

#ifdef RB_MATERIAL_AMBIENT_MAP
uniform sampler2D rb_materialAmbientMap;
#endif

#ifdef RB_MATERIAL_DIFFUSE_MAP
uniform sampler2D rb_materialDiffuseMap;
#endif

varying vec3 vertexAmbientColor;
varying vec3 vertexDiffuseColor;
varying vec2 vertexUV;

void main() {

    vec3 ambientColorLinear = vertexAmbientColor;
#ifdef RB_MATERIAL_AMBIENT_MAP
    ambientColorLinear *= gammaToLinear(texture2D(rb_materialAmbientMap, vertexUV).rgb, 2.2);
#endif

    vec3 diffuseColorLinear = vertexDiffuseColor;
#ifdef RB_MATERIAL_DIFFUSE_MAP
    diffuseColorLinear *= gammaToLinear(texture2D(rb_materialDiffuseMap, vertexUV).rgb, 2.2);
#endif

    vec3 colorLinear = ambientColorLinear + diffuseColorLinear;

    vec3 colorGammaCorrected = linearToGamma(colorLinear, screenGamma);
    vec4 fragmentColor = vec4(colorGammaCorrected, 1);

    gl_FragColor = fragmentColor;

}

)";
