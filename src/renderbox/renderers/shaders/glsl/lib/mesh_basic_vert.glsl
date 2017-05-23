R"(

#version 330

uniform mat4 worldProjectionMatrix;

out gl_PerVertex {
    vec4 gl_Position;
};

in vec3 vertexPosition;

void main() {
    gl_Position = worldProjectionMatrix * vec4(vertexPosition, 1.0f);
}

)"