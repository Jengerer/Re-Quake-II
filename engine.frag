in vec3 ex_colour;
out vec4 gl_frag_colour;

void main(void) {
	gl_frag_colour = vec4(ex_colour, 1.0);
}
