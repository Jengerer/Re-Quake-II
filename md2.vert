#version 120

// Input and output attributes.
attribute vec3 position0;
attribute vec3 normal0;
attribute vec3 position1;
attribute vec3 normal1;
attribute vec2 uv;
varying vec3 psNormal;
varying vec2 psUV;

// Camera view variables.
uniform mat4 object;
uniform mat4 projectionView;

// Animation interpolation amount.
uniform float time;

// Pass vertex colour to next shader.
void main(void) {
	vec4 translated = object * vec4(position0, 1.f);
	gl_Position = projectionView *
		object *
		vec4(mix(position0, position1, object[0][0]), 1.f);
	psNormal = mix(normal0, normal1, time);
	psUV = uv;
}
