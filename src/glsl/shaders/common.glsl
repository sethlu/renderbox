R"(

#ifndef RENDERBOX_SHADER_COMMON
#define RENDERBOX_SHADER_COMMON

#define PI 3.14159265359

vec3 gammaToLinear(vec3 value, float gamma) {
    return pow(value, vec3(gamma));
}

vec3 linearToGamma(vec3 value, float gamma) {
    return pow(value, vec3(1.0 / gamma));
}

#define saturate(v) \
	clamp((v), 0.0, 1.0)

vec3 dehomogenize(vec4 vector) {
    return vector.xyz / vector.w;
}

#endif

)"
