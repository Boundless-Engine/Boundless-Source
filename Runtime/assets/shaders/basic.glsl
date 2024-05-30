#version 450

#ifdef VERTEX_SHADER
layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec3 i_Color;

layout(location = 0) out vec3 o_Color;

void main()
{
    gl_Position = vec4(i_Position, 1.0);
    o_Color = i_Color;
}
#elif defined(FRAGMENT_SHADER)

layout(location = 0) in vec3 o_Color;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(o_Color, 1.0);
}

#endif

