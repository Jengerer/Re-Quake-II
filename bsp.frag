#version 150

in vec3 ps_normal;
out vec4 frag_colour;

// Texture variables.
uniform sampler2D texture2d;
uniform vec4 colour;

void main(void) {
	float toEnd = (4096.f - ps_normal.z) / (4096.f - 4.f);
	float depth = max(pow(toEnd, 10.f), colour.a);
	frag_colour = vec4(colour.xyz * depth, 1.f);
}
