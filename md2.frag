#version 150

in vec3 psNormal;
in vec2 psUV;
out vec4 fragColour;

// Texture variables.
uniform sampler2D texture;

void main(void) {
	fragColour = vec4(texture2D(texture, psUV).rgb, length(psNormal));
}
