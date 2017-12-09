R"(

#version 330

uniform vec3 rb_materialColor;

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(rb_materialColor, 1.0);
}

)"