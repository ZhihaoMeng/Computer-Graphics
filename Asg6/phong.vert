#version 330 core

layout (location=0) in vec3 position;
layout (location=2) in vec3 normal;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

out vec3 P_world;
out vec3 N_world;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	
	P_world = (M*vec4(position, 1.0)).xyz;
	// N_world = mat3(transpose(inverse(M)))*normal;
	N_world = mat3(transpose(inverse(M)))*(-normal);
	// N_world = -normal;
	// N_world = (M*-vec4(normal, 1.0)).xyz;
}
