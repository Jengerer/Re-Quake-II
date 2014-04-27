#version 150
in vec3 ex_colour;

void main(void) {
	gl_FragColor = vec4(ex_colour, 1.0);
}
