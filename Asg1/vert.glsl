#version 330 core

layout (location = 0) in vec3 position; // position has attribute

out vec4 vert_color;                    // output to fragment shader

void main()
{
  gl_Position = vec4(position.x, position.y, position.z, 1.0);

  vert_color = vec4(0.5, 0.0, 0.0, 1.0);
}
