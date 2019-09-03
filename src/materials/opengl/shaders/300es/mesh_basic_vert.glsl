R"(

#version 300 es

precision mediump float;

#include <lights_preamble>

uniform mat4 rb_worldProjectionMatrix;
uniform vec3 rb_materialDiffuseColor;

out vec3 vertexColor;

in vec3 rb_vertexPosition;

void main() {

    // Position

    #include <glposition_vert>

    // Lighting

    vertexColor = rb_materialDiffuseColor;

}

)";
