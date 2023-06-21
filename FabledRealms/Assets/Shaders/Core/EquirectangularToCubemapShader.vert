#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 localPos;
out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = aPos;
    WorldPos = localPos;
    gl_Position =  projection * view * vec4(localPos, 1.0);
}