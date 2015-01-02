#version 150

// Input and output attributes.
in vec3 position;
in vec2 uv;
in vec2 lightMapUV;
out vec2 psUV;

// Camera view variables.
uniform mat4 view;
uniform mat4 projection;

// Pass vertex colour to next shader.
void main(void) {
	gl_Position = projection *
		view *
		vec4(position, 1.f);
	psUV = (uv / 64.f) + lightMapUV; // Lightmap is (0, 0) for now.
}