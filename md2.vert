#version 150

// Input and output attributes.
in vec3 position0;
in vec3 normal0;
in vec3 position1;
in vec3 normal1;
in vec2 uv;
out vec3 psNormal;
out vec2 psUV;

// Camera view variables.
uniform mat4 object;
uniform mat4 view;
uniform mat4 projection;

// Animation interpolation amount.
uniform float time;

// Pass vertex colour to next shader.
void main(void) {
	vec4 translated = object * vec4(position0, 1.f);
	gl_Position = projection *
		view *
		object *
		vec4(mix(position0, position1, time), 1.f);
	psNormal = mix(normal0, normal1, time);
	psUV = uv;
}
