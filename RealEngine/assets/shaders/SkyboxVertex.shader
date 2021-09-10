#version 330 core
layout(location = 0) in vec3 a_Position;

out vec3 TextureCoordinates;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    TextureCoordinates = vec3(vec4(a_Position, 1.0));
    vec4 Position = u_Projection * u_View * vec4(a_Position, 1.0);
    gl_Position = Position.xyww;
}