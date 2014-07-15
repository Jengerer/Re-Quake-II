#version 150

in vec3 ex_colour;
out vec4 frag_colour;

void main(void) {
	frag_colour = vec4(ex_colour, 1.0);
}
