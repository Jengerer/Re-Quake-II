#version 150
in vec3 ex_colour;
out vec4 gl_FragColor;

void main(void) {
	gl_FragColor = vec4(ex_colour, 1.0);
}