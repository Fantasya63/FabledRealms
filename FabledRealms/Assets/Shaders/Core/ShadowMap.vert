#version 460 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 a_ModelMatrix;

void main()
{
    gl_Position = a_ModelMatrix * vec4(a_Pos, 1.0);
}  