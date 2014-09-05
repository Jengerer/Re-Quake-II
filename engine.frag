#version 150

in vec2 ex_texture;
out vec4 frag_colour;

// Texture variables.
uniform sampler2D texture2d;

void main(void) {
	frag_colour = texture(texture2d, ex_texture);
}
