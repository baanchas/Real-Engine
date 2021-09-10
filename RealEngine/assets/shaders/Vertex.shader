#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;
layout(location = 4) in float a_FlatColor;
layout(location = 5) in float a_TextureIndex;
layout(location = 6) in float a_TilingFactor;
layout(location = 7) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_MeshTransform;


uniform vec3 u_Albedo;
uniform float u_Metallness;
uniform float u_Roughness;
uniform float u_AO;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_Albedo;
out float v_Metallic;
out float v_Roughness;
out float v_AO;
out float v_TextureIndex;
out float v_TilingFactor;
out flat int v_EntityID;
out float v_FlatColor;
out mat4 v_MeshTransform;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_Albedo = u_Albedo;
    v_Metallic = u_Metallness;
    v_Roughness = u_Roughness;
    v_AO = u_AO;
    v_TextureIndex = a_TextureIndex;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
    v_FlatColor = a_FlatColor;
    v_MeshTransform = u_MeshTransform;
    v_Normal = mat3(transpose(inverse(u_MeshTransform))) * a_Normal;
    gl_Position = u_ViewProjection * (u_MeshTransform * vec4(a_Position, 1.0f));
}