#version 450

#ifdef VERTEX_SHADER

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in float i_ID;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec3 o_Normal;
layout(location = 2) out vec3 o_Position;
layout(location = 3) out vec3 o_LightColor;
layout(location = 4) out vec3 o_LightPosition;
layout(location = 5) out float o_ID;

struct Light {
    vec3 position;
    vec3 color;
};

layout(binding = 0) uniform UBO {
    mat4 view;
    mat4 projection;
} ubo;

layout(push_constant) uniform PushConstants
{
    mat4 model;
    vec4 baseColor;
    Light light;
} pushConstants;


void main()
{
    gl_Position = ubo.projection * ubo.view * pushConstants.model * vec4(i_Position, 1.0);
    
    o_Color = pushConstants.baseColor;
    o_Normal = i_Normal;
    o_LightColor = pushConstants.light.color;
    o_LightPosition = pushConstants.light.position;
    o_Position =  vec3(pushConstants.model * vec4(i_Position, 1.0));
    o_ID = i_ID;
}


#endif

#ifdef FRAGMENT_SHADER

layout(location = 0) in vec4 i_Color;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec3 i_Position;
layout(location = 3) in vec3 i_LightColor;
layout(location = 4) in vec3 i_LightPosition;
layout(location = 5) in float i_ID;

layout(location = 0) out vec4 o_Color;// color attachment 
layout(location = 1) out vec4 o_ID;// id attachment

void main()
{
    // Ambient lighting
    float amb = 0.1;
    vec3 ambient = amb * i_LightColor;

    // Diffuse lighting
    vec3 norm = normalize(i_Normal);
    vec3 lightDir = normalize(i_LightPosition - i_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * i_LightColor;

    // Final color calculation
    vec3 result = (ambient + diffuse) * vec3(i_Color);

    o_Color = vec4(result, 1.0);
    o_ID = vec4(i_ID, 0, 0, 1);
}

#endif

