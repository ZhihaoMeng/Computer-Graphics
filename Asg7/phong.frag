#version 330 core

in vec3 P_world;
in vec3 N_world;
in vec2 TexCoord;

out vec4 frag_color;

uniform vec4 color;// surface color
uniform vec3 L_world;// light pos
uniform vec3 V_world; // camera pos
uniform sampler2D OurTexture;

void main()
{
	// light intensity for ambient, diffuse, specular component
	vec4 Ia = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 Id = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 Is = vec4(1.0, 1.0, 1.0, 1.0);

	// material property
	vec4 ka = vec4(0.4, 0.4, 0.4, 1.0);
	vec4 kd = vec4(0.8, 0.8, 0.8, 1.0);
	vec4 ks = vec4(0.9, 0.9, 0.9, 1.0);

	float n = 32.0;  // shinings in theory
	vec3 N = normalize(vec3(N_world));
	vec3 L = normalize(vec3(L_world - P_world));

	// ambient color
	// frag_color = (ka * Ia) * color;
	
	Id = Id * clamp(dot(N, L),0.0, 1.0);
	// frag_color = (ka * Ia + kd * Id)*color
	vec3 V = normalize(V_world - P_world);

	vec3 R = normalize(reflect(-L, N));
	// 2.0 * N * dot(N, L) - L;

	// (R * V)^n shinines
	Is = Is * pow(max(dot(R,V),0.0), n);

	//frag_color = (ka *Ia + kd *Id + ks *Is)*color;
	frag_color = (ka * Ia + kd * Id + ks * Is) * texture(OurTexture, TexCoord);
}
