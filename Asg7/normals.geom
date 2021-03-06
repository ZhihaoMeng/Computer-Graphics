#version 330 core

layout (triangles) in;

// can only output points, line_strip, or triangle_strip
// layout (line_strip, max_vertices=6) out;
layout (line_strip, max_vertices=8) out;

uniform mat4 mvp;

in vec4 vert_normal[];
in vec4 vert_color[];

out vec4 vertex_color;

void main()
{
    // 3 lines for the 3 vertex normals (1 line per vertex, hence 6 verts out)
    for (int i = 0; i < gl_in.length(); i++) {

	vec4 p = gl_in[i].gl_Position;
	vec4 n = -vert_normal[i];

	gl_Position = mvp * p;
	vertex_color = vert_color[i];
	EmitVertex();

	gl_Position = mvp * (p + 0.1 * n);
	vertex_color = vert_color[i];
	EmitVertex();

	EndPrimitive();
    }

    // 1 line for the face normal (hence 2 verts out)
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;  
    vec3 p2 = gl_in[2].gl_Position.xyz; 

    vec3 v1 = p1 - p0;
    vec3 v2 = p2 - p0;

    // face normal
    vec3 fn = normalize(cross(v1, v2));

    // face center
    vec3 fc = (p0 + p1 + p2)/3.0;

    gl_Position = mvp * vec4(fc, 1.0);
    vertex_color = vec4(1, 1, 1, 1);
    EmitVertex();

    gl_Position = mvp * vec4(fc + 0.3 * fn, 1.0);
    vertex_color = vec4(1, 1, 1, 1);
    EmitVertex();

    EndPrimitive();
}
