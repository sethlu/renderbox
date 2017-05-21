R"(

#version 330

uniform vec3 diffuse;

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(diffuse, 1.0);
}

)"