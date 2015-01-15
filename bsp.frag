#version 120

varying vec2 psUV;

// Texture variables.
uniform sampler2D texture;

void main(void) {
	gl_FragColor = texture2D(texture, psUV);
}
