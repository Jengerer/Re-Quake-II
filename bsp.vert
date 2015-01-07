#version 150

// Input and output attributes.
in vec3 position;
in vec2 uv;
in vec2 lightMapUV;
out vec2 psUV;

// Texture size uniform.
uniform vec2 textureSize;

// Camera view variables.
uniform mat4 projectionView;

// Pass vertex colour to next shader.
void main(void) {
	gl_Position = projectionView * vec4(position, 1.f);
	psUV = (uv / textureSize) + lightMapUV; // Lightmap is (0, 0) for now.
}