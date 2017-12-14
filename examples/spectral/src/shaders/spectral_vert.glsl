
#version 330

uniform mat4 rb_worldProjectionMatrix;
uniform mat4 rb_worldMatrix;
uniform mat4 rb_worldNormalMatrix;

out gl_PerVertex {
    vec4 gl_Position;
};
out vec3 vertexWorldPosition;
out vec3 vertexWorldNormal;

in vec3 rb_vertexPosition;
in vec3 rb_vertexNormal;

void main() {

    vertexWorldPosition = vec3(rb_worldMatrix * vec4(rb_vertexPosition, 1));
    vertexWorldNormal = vec3(rb_worldNormalMatrix * vec4(rb_vertexNormal, 0));

    gl_Position = rb_worldProjectionMatrix * vec4(rb_vertexPosition, 1);

}
