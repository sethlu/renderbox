R"(

#version 330

#include <lights_preamble>

uniform mat4 rb_worldProjectionMatrix;
uniform mat4 rb_worldMatrix;
uniform mat4 rb_worldNormalMatrix;
uniform vec3 rb_materialDiffuseColor;

out vec3 vertexDiffuseColor;
out vec2 vertexUV;

in vec3 rb_vertexPosition;
in vec3 rb_vertexNormal;
in vec2 rb_vertexUV;

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

    vertexDiffuseColor = vec3(0.0);

    #if RB_NUM_POINT_LIGHTS > 0

    vec3 N = vertexWorldNormal;

    for (int i = 0; i < rb_numActivePointLights; ++i) {

        vec3 lightVector = rb_pointLights[i].position - vertexWorldPosition;
        vec3 L = normalize(lightVector);
        float lightDistance = length(lightVector);

        vertexDiffuseColor += rb_materialDiffuseColor * rb_pointLights[i].color *
            max(dot(N, L), 0.0) / (1.0 + (0.25 * lightDistance * lightDistance));

    }

    #endif

}

)"