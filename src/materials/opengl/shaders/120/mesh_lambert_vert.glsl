R"(

#version 120

#include <common>
#include <lights_preamble>

uniform mat4 rb_worldProjectionMatrix;
uniform mat4 rb_worldMatrix;
uniform mat3 rb_worldNormalMatrix;
uniform vec3 rb_sceneAmbientColor;
uniform vec3 rb_materialAmbientColor;
uniform vec3 rb_materialDiffuseColor;

varying vec3 vertexAmbientColor;
varying vec3 vertexDiffuseColor;
varying vec2 vertexUV;

attribute vec3 rb_vertexPosition;
attribute vec3 rb_vertexNormal;
attribute vec2 rb_vertexUV;

void main() {

    #include <begin_vert>

    // Position

    vec3 vertexWorldPosition;

    #include <worldposition_vert>
    #include <glposition_vert>

    // Normal

    vec3 vertexWorldNormal;

    #include <worldnormal_vert>

    // UV

    vertexUV = rb_vertexUV;

    // Lighting

    // Ambient

    vertexAmbientColor = rb_sceneAmbientColor * rb_materialAmbientColor;

    // Diffuse

    vertexDiffuseColor = vec3(0.0);

    #if RB_NUM_POINT_LIGHTS > 0

    vec3 N = vertexWorldNormal;

    for (int i = 0; i < rb_numActivePointLights; ++i) {

        vec3 lightVector = rb_pointLights[i].position - vertexWorldPosition;
        vec3 L = normalize(lightVector);
        float lightDistance = length(lightVector);

        vertexDiffuseColor += rb_pointLights[i].color *
            max(dot(N, L), 0.0) / (1.0 + (0.25 * lightDistance * lightDistance));

    }

    vertexDiffuseColor *= rb_materialDiffuseColor;

    #endif

}

)";
