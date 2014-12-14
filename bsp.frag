#version 150

in vec3 ps_normal;
out vec4 frag_colour;

// Texture variables.
uniform sampler2D texture2d;

void main(void) {
	float depth = ps_normal.z * 0.5f + 0.5f; // Convert from [-1, 1] to [0, 1].
	frag_colour = vec4(depth, depth, depth, 1.f);
}
