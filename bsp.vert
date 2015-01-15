#version 120

// Input and output attributes.
attribute vec3 position;
attribute vec2 uv;
attribute vec2 lightMapUV;
varying vec2 psUV;

// Texture size uniform.
uniform vec2 textureSize;

// Camera view variables.
uniform mat4 projectionView;

// Pass vertex colour to next shader.
void main(void) {
	gl_Position = projectionView * vec4(position, 1.f);
	psUV = (uv / textureSize) + lightMapUV; // Lightmap is (0, 0) for now.
}
