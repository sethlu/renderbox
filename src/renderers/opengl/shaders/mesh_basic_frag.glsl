R"(

#version 330

uniform vec3 rb_vertexColor;

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(rb_vertexColor, 1.0);
}

)"