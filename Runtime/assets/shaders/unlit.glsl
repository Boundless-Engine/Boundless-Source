#version 450

#ifdef VERTEX_SHADER

layout(location = 0) in vec3 i_Position;
layout(location = 1) in float i_ID;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_ID;

layout(binding = 0) uniform UBO {
    mat4 view;
    mat4 projection;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 model;
    vec4 color;
} pushConstants;

void main() {
    gl_Position = ubo.projection * ubo.view * pushConstants.model * vec4(i_Position, 1.0);
    o_Color = pushConstants.color;
    o_ID = vec4(i_ID, 0, 0, 1);
}

#endif

#ifdef FRAGMENT_SHADER

layout(location = 0) in vec4 i_Color;
layout(location = 1) in float i_ID;

layout(location = 0) out vec4 o_Color; // color attachment 
layout(location = 1) out float o_ID; // id attachment

void main() {
    o_Color = i_Color;
    o_ID = i_ID;
}
#endif
