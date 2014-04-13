#version 150
in vec3 in_pos;
 
// Pass vertex colour to next shader.
out vec3 ex_colour;
void main(void) {
	gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
    ex_colour = vec3(1.0, 1.0, 1.0);
}