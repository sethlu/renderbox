R"(

#version 330

const float screenGamma = 2.2;

uniform vec3 vertexColor;

out vec4 fragmentColor;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

void main() {

    vec3 lightWorldPosition = vec3(2, 2, 20);

    vec3 N = normalize(vertexWorldNormal);
    vec3 L = normalize(lightWorldPosition - vertexWorldPosition);
    float distance = length(lightWorldPosition - vertexWorldPosition);

    vec3 diffuse = vertexColor * max(dot(N, L), 0) * 40 * (1.0 / (1.0 + (0.25 * distance * distance)));

    vec3 colorLinear = diffuse;
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0 / screenGamma));
    fragmentColor = vec4(colorGammaCorrected, 1.0);

}

)"