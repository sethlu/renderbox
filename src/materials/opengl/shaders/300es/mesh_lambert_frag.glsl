R"(

#version 300 es

precision mediump float;

#include <common>

const float screenGamma = 2.2;

#ifdef RB_MATERIAL_AMBIENT_MAP
uniform sampler2D rb_materialAmbientMap;
#endif
#ifdef RB_MATERIAL_DIFFUSE_MAP
uniform sampler2D rb_materialDiffuseMap;
#endif

out vec4 fragmentColor;

in vec3 vertexAmbientColor;
in vec3 vertexDiffuseColor;
in vec2 vertexUV;

void main() {

    vec3 ambientColorLinear = vertexAmbientColor;
	#ifdef RB_MATERIAL_AMBIENT_MAP
	ambientColorLinear *= gammaToLinear(texture(rb_materialAmbientMap, vertexUV).rgb, 2.2);
	#endif
	
	vec3 diffuseColorLinear = vertexDiffuseColor;
	#ifdef RB_MATERIAL_DIFFUSE_MAP
	diffuseColorLinear *= gammaToLinear(texture(rb_materialDiffuseMap, vertexUV).rgb, 2.2);
	#endif

    vec3 colorLinear = ambientColorLinear + diffuseColorLinear;

    vec3 colorGammaCorrected = linearToGamma(colorLinear, screenGamma);
    fragmentColor = vec4(colorGammaCorrected, 1);

}

)";
