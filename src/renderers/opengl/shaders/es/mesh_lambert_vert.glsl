R"(

#version 300 es

precision mediump float;

#include <lights_preamble>

uniform mat4 rb_worldProjectionMatrix;
uniform mat4 rb_worldMatrix;
uniform mat4 rb_worldNormalMatrix;
uniform vec3 rb_sceneAmbientColor;
uniform vec3 rb_materialAmbientColor;
uniform vec3 rb_materialDiffuseColor;

out vec3 vertexColor;

in vec3 rb_vertexPosition;
in vec3 rb_vertexNormal;

void main() {

    #include <begin_vert>

    // Position

    vec3 vertexWorldPosition;

    #include <worldposition_vert>
    #include <glposition_vert>

    // Normal

    vec3 vertexWorldNormal;

    #include <worldnormal_vert>

    // Lighting

    vertexColor = rb_sceneAmbientColor * rb_materialAmbientColor;

    #if RB_NUM_POINT_LIGHTS > 0

    vec3 N = vertexWorldNormal;

    for (int i = 0; i < rb_numActivePointLights; ++i) {

        vec3 lightVector = rb_pointLights[i].position - vertexWorldPosition;
        vec3 L = normalize(lightVector);
        float lightDistance = length(lightVector);

        vertexColor += rb_materialDiffuseColor * rb_pointLights[i].color *
            max(dot(N, L), 0.0) / (1.0 + (0.25 * lightDistance * lightDistance));

    }

    #endif

}

)"