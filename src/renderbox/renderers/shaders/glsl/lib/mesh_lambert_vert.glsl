R"(

#version 330

uniform mat4 worldProjectionMatrix;
uniform mat4 worldMatrix;
uniform mat4 worldNormalMatrix;

out gl_PerVertex {
    vec4 gl_Position;
};
out vec3 vertexWorldPosition;
out vec3 vertexWorldNormal;

in vec3 vertexPosition;
in vec3 vertexNormal;

void main() {
    vertexWorldPosition = vec3(worldMatrix * vec4(vertexPosition, 1.0));
    vertexWorldNormal = vec3(worldNormalMatrix * vec4(vertexNormal, 0.0));

    gl_Position = worldProjectionMatrix * vec4(vertexPosition, 1.0);
}

)"