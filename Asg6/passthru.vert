#version 330 core

layout (location = 0) in vec3 position; // position as attribute
layout (location = 2) in vec3 normal;   // normal as attribute

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0);
}
