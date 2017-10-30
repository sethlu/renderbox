R"(

#version 300 es

precision mediump float;

uniform mat4 worldProjectionMatrix;

in vec3 vertexPosition;

void main() {
    gl_Position = worldProjectionMatrix * vec4(vertexPosition, 1.0f);
}

)"
