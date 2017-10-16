R"(

#version 300 es

precision mediump float;

uniform mat4 worldProjectionMatrix;
uniform mat4 worldMatrix;
uniform mat4 worldNormalMatrix;

out vec3 vertexWorldPosition;
out vec3 vertexWorldNormal;

in vec3 vertexPosition;
in vec3 vertexNormal;

void main() {
    vertexWorldPosition = vec3(worldMatrix * vec4(vertexPosition, 1));
    vertexWorldNormal = vec3(worldNormalMatrix * vec4(vertexNormal, 0));

    gl_Position = worldProjectionMatrix * vec4(vertexPosition, 1);
}

)"
