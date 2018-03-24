R"(

#version 300 es

precision mediump float;

#include <common>

const float screenGamma = 2.2;

uniform vec3 rb_sceneAmbientColor;
uniform vec3 rb_materialAmbientColor;
uniform sampler2D rb_materialDiffuseMap;

out vec4 fragmentColor;

in vec3 vertexDiffuseColor;
in vec2 vertexUV;

void main() {

    vec3 ambientColorLinear = rb_sceneAmbientColor * rb_materialAmbientColor;
	
	vec3 diffuseColorLinear = vertexDiffuseColor;
	#ifdef RB_MATERIAL_DIFFUSE_MAP
	diffuseColorLinear *= gammaToLinear(texture(rb_materialDiffuseMap, vertexUV).rgb, 2.2);
	#endif

    vec3 colorLinear = ambientColorLinear + diffuseColorLinear;

    vec3 colorGammaCorrected = linearToGamma(colorLinear, screenGamma);
    fragmentColor = vec4(colorGammaCorrected, 1);

}

)"
