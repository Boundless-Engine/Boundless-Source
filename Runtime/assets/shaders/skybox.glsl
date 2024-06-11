#version 450

#ifdef VERTEX_SHADER


layout(location = 0) in vec3 i_Position;

layout(location = 0) out vec4 o_TopColor;
layout(location = 1) out vec4 o_BottomColor;
layout(location = 2) out float o_Height;

layout(binding = 0) uniform UBO {
    mat4 view;
    mat4 projection;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 model;
    vec4 topColor;
    vec4 bottomColor;
    float height;
}constants;


void main(){
    gl_Position = ubo.projection * ubo.view * constants.model * vec4(i_Position, 1.0);
    
    o_TopColor = constants.topColor;
    o_BottomColor = constants.bottomColor;

    o_Height = constants.height;
}
#endif

#ifdef FRAGMENT_SHADER

layout(location = 0) in vec4 i_TopColor;
layout(location = 1) in vec4 i_BottomColor;
layout(location = 2) in float  i_Height;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_ID;

void main() 
{
    float t = (gl_FragCoord.y / i_Height);
    o_Color =  mix(i_TopColor, i_BottomColor, t);
    o_ID = vec4(-1.0, 0, 0, 1);
}
#endif