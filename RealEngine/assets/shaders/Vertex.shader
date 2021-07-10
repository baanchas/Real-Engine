#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexID;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;
layout(location = 6) in mat4 a_Matrix;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexID;
out float v_TilingFactor;
out flat int v_EntityID;
void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexID = a_TexID;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * (a_Matrix * vec4(a_Position, 1.0f));
}