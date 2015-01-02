#version 150

in vec2 psUV;
out vec4 frag_colour;

// Texture variables.
uniform sampler2D texture;

void main(void) {
	frag_colour = texture2D(texture, psUV);
}
