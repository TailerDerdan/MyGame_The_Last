#version 330 core

out vec4 FragColor;

in vec2 i_tex;

uniform sampler2D texture;
uniform vec2 mousePosition;
uniform vec2 resolution;

void main() {
	vec2 uv	   = gl_FragCoord.xy / resolution;
	vec2 mouse = mousePosition   / resolution;

	mouse.y    = 1.0 - mouse.y;

	vec4 pixel = texture2D(texture, i_tex);

	float dist = length(uv - mouse);
	float circle = 1.0 - dist * 10;

	if (pixel.rgb == vec3(1.0))
	{
		FragColor = vec4(1.000, 0.710, 0.000, circle * 0.25);
	}
	if (pixel.rgb == vec3(0.0))
	{
		FragColor = pixel;
	}
};