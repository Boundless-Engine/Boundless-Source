#version 450 

#ifdef VERTEX_SHADER

void main(){}

#endif

#ifdef FRAGMENT_SHADER

layout (location=0) int vec3 i_Color;
layout (location=0) out vec4 o_Color;
void main(){ o_Color = vec4(i_Color); }


#endif