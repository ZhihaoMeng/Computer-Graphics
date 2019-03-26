#version 330 core

layout (location = 0) in vec3 position; // position as attribute
layout (location = 2) in vec3 normal;   // normal as attribute

out vec4 vert_normal;
out vec4 vert_color;

uniform mat4 mvp;

void main()
{
    gl_Position = vec4(position, 1.0);
    vert_normal = vec4(normal,1.0);
    vert_color = vec4(0.5, 0.0, 0.0, 1.0);
}
