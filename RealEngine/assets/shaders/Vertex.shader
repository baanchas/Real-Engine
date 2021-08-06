#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;
layout(location = 4) in vec3 a_Albedo;
layout(location = 5) in float a_Metallic;
layout(location = 6) in float a_Roughness;
layout(location = 7) in float a_AO;
layout(location = 8) in float a_TexID;
layout(location = 9) in float a_TilingFactor;
layout(location = 10) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 MeshTransform;
uniform int EntityID;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexID;
out float v_TilingFactor;
out flat int v_EntityID;
out vec3 v_Normal;
out vec3 FragPos;
out vec3 v_Albedo;
out float v_Metallic;
out float v_Roughness;
out float v_AO;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexID = a_TexID;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = EntityID;
    v_Albedo = a_Albedo;
    v_Metallic = a_Metallic;
    v_Roughness = a_Roughness;
    v_AO = a_AO;
    v_Normal = mat3(transpose(inverse(MeshTransform))) * a_Normal;
    //v_Normal = a_Normal;
    FragPos = vec3(MeshTransform * vec4(a_Position, 1.0f));
    gl_Position = u_ViewProjection * (MeshTransform * vec4(a_Position, 1.0f));
}