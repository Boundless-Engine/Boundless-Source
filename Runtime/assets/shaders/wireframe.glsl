#version 450

#ifdef VERTEX_SHADER

layout(binding = 0) uniform UBO {
    mat4 view;
    mat4 projection;
} ubo;
layout(push_constant) uniform PushConstants
{
    mat4 model;
} pushConstants;

layout(location=0) in vec3 i_Position;

void main()
{
    gl_Position = ubo.projection * ubo.view * pushConstants.model * vec4(i_Position, 1.0);    
}

#elif defined(FRAGMENT_SHADER)

layout(location=0) out vec4 o_Color;

void main() {
    o_Color = vec4(0,0, 0, 1);
}

#endif