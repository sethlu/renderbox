R"(

#version 300 es

precision mediump float;

uniform vec3 vertexColor;

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(vertexColor, 1.0);
}

)"
