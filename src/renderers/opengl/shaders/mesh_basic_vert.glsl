R"(

#version 330

uniform mat4 rb_worldProjectionMatrix;

out gl_PerVertex {
    vec4 gl_Position;
};

in vec3 rb_vertexPosition;

void main() {
    gl_Position = rb_worldProjectionMatrix * vec4(rb_vertexPosition, 1.0f);
}

)"