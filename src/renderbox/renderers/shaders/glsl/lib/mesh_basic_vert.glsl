R"(

#version 330

uniform mat4 worldProjectionMatrix;

out gl_PerVertex {
    vec4 gl_Position;
};

out vec3 fragmentDiffuse;

in vec3 vertexPosition;

void main() {
    gl_Position = worldProjectionMatrix * vec4(vertexPosition, 1.0f);
}

)"