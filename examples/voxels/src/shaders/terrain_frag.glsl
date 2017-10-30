
#version 330

const float screenGamma = 2.2;

uniform vec3 sceneAmbientColor;

out vec4 fragmentColor;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

void main() {

    vec3 lightWorldPosition = vec3(2, 2, 60);

    vec3 N = normalize(vertexWorldNormal);
    vec3 L = normalize(lightWorldPosition - vertexWorldPosition);
    float distance = length(lightWorldPosition - vertexWorldPosition);

    vec3 vertexColor = vec3(1);

    vec3 diffuseColor = max(dot(N, L), 0) * vec3(1, 1, 1) * 500 / (1 + (0.25 * distance * distance));

    vec3 colorLinear = vertexColor * (sceneAmbientColor + diffuseColor);
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1 / screenGamma));
    fragmentColor = vec4(colorGammaCorrected, 1);

}
