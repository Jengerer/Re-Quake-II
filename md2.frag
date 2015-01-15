#version 120

varying vec3 psNormal;
varying vec2 psUV;

// Texture variables.
uniform sampler2D texture;

void main(void) {
	gl_FragColor  = vec4(texture2D(texture, psUV).rgb, length(psNormal));
}
