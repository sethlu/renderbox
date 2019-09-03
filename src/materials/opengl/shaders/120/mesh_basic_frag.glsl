R"(

#version 120

const float screenGamma = 2.2;

varying vec3 vertexColor;

void main() {

    vec3 colorLinear = vertexColor;
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0 / screenGamma));
    vec4 fragmentColor = vec4(colorGammaCorrected, 1);

    gl_FragColor = fragmentColor;

}

)";
