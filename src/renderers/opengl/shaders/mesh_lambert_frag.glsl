R"(

#version 330

const float screenGamma = 2.2;

uniform vec3 rb_sceneAmbientColor;
uniform vec3 rb_vertexColor;

out vec4 fragmentColor;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

void main() {

    vec3 lightWorldPosition = vec3(2, 2, 40);

    vec3 N = normalize(vertexWorldNormal);
    vec3 L = normalize(lightWorldPosition - vertexWorldPosition);
    float distance = length(lightWorldPosition - vertexWorldPosition);

    vec3 diffuseColor = max(dot(N, L), 0) * vec3(1) * 400 / (1 + (0.25 * distance * distance));

    vec3 colorLinear = rb_vertexColor * (rb_sceneAmbientColor + diffuseColor);
    vec3 colorGammaCorrected = pow(colorLinear, vec3(1 / screenGamma));
    fragmentColor = vec4(colorGammaCorrected, 1);

}

)"