R"(

#version 330

const float screenGamma = 2.2;

out vec4 fragmentColor;

in vec3 vertexColor;

void main() {

    vec3 colorLinear = vertexColor;
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1 / screenGamma));
    fragmentColor = vec4(colorGammaCorrected, 1);

}

)"