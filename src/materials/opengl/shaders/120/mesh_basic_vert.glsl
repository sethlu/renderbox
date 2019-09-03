R"(

#version 120

#include <lights_preamble>

uniform mat4 rb_worldProjectionMatrix;
uniform vec3 rb_materialDiffuseColor;

varying vec3 vertexColor;

attribute vec3 rb_vertexPosition;

void main() {

    // Position

    #include <glposition_vert>

    // Lighting

    vertexColor = rb_materialDiffuseColor;

}

)";
