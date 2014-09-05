#version 150

// Input and output attributes.
in vec3 in_vertex;
in vec2 in_texture;
out vec2 ex_texture;

// Camera view variables.
uniform mat4 object;
uniform mat4 view;
uniform mat4 projection;

// Pass vertex colour to next shader.
void main(void) {
	gl_Position = projection *
		// view *
		object *
		vec4(in_vertex, 1.0);
    ex_texture = in_texture;
}
