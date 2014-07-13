#version 150

// Input and output attributes.
in vec3 in_vertex;
in vec2 in_texture;
out vec3 ex_colour;

// Camera view variables.
uniform mat4 transform;
uniform mat4 projection;

// Pass vertex colour to next shader.
void main(void) {
	gl_Position = projection *
		transform *
		vec4(in_vertex, 1.0);
    ex_colour = vec3(in_texture, 0.0);
}