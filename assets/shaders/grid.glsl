#define type vertex
#version 450

const vec2 origin = vec2(0, 0);
const float blend_factor = 0.5;

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

out vec4 v_Color;

uniform mat4 u_VP;

void main()
{
    gl_Position = u_VP * vec4(a_Position, 1.0);

    float dist = distance(origin, vec2(a_Position.x, a_Position.z));
    float alpha = exp(-dist * blend_factor);

    v_Color = vec4(a_Color, alpha);
}

#define type fragment
#version 450

layout (location = 0) out vec4 out_color;

in vec4 v_Color;

void main()
{
    out_color = v_Color;
}
