#version 330

uniform mat4 worldProjectionMatrix;

out gl_PerVertex {
    vec4 gl_Position;
};

out vec4 vertexColor;

in vec3 vertexPosition;

void main() {
    gl_Position = worldProjectionMatrix * vec4(vertexPosition, 1.0f);
    vertexColor = vec4(0.7 + sin(vertexPosition / 4) * 0.2, 1.0f);
}