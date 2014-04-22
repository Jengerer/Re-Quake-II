#version 150

uniform float timer1;
out vec3 ex_colour;

// Create perspective projection matrix.
mat4 view_frustrum(
	float angle_of_view,
	float aspect_ratio,
	float z_near,
	float z_far)
{
	return mat4(
		vec4(1.0 / tan(angle_of_view), 0.0, 0.0, 0.0),
		vec4(0.0, aspect_ratio/tan(angle_of_view), 0.0, 0.0),
		vec4(0.0, 0.0, (z_far + z_near)/(z_far - z_near), 1.0),
		vec4(0.0, 0.0, -2.0 * z_far * z_near / (z_far - z_near), 0.0)
	);
}

// Scale matrix.
mat4 scale(
	float x_scale,
	float y_scale,
	float z_scale
)
{
	return mat4(
		vec4(x_scale, 0.0, 0.0, 0.0),
		vec4(0.0, y_scale, 0.0, 0.0),
		vec4(0.0, 0.0, z_scale, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);
}
 
// Pass vertex colour to next shader.
void main(void) {
	gl_Position = view_frustrum(radians(45.0), 4.0 / 3.0, 1.0, 100.0)
		* scale(timer1, timer1, timer1)
		* gl_Vertex;
    ex_colour = vec3(1.0, 1.0, 1.0);
}
