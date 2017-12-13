R"(

#ifndef RENDERBOX_SHADER_LIGHTS_PREAMBLE
#define RENDERBOX_SHADER_LIGHTS_PREAMBLE

#if RB_NUM_POINT_LIGHTS > 0

struct RB_PointLight {
    vec3 position; // World position
    vec3 color;
};

uniform RB_PointLight rb_pointLights[RB_NUM_POINT_LIGHTS];

uniform int rb_numActivePointLights;

#endif

#endif

)"