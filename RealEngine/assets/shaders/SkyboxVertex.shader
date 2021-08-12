#version 330 core
layout(location = 0) in vec3 a_Position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = vec3(vec4(a_Position, 1.0));
    vec4 position = projection * view * vec4(a_Position, 1.0);
    gl_Position = position.xyww;
}