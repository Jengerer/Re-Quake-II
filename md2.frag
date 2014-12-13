#version 150

in vec3 ps_normal;
out vec4 frag_colour;

// Texture variables.
uniform sampler2D texture2d;

void main(void) {
	frag_colour = vec4(ps_normal, 1.f);
}
