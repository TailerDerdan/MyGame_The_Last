#version 330 core

out vec4 FragColor;

in vec2 i_tex;

uniform sampler2D texture;
uniform vec2 mousePosition; // ѕозици€ курсора мыши, переданна€ из приложени€
uniform vec2 resolution; // –азрешение окна, переданное из приложени€

void main() {
	vec2 uv	   = gl_FragCoord.xy / resolution;
	vec2 mouse = mousePosition   / resolution;

	mouse.y    = 1.0 - mouse.y;

	vec4 pixel = texture2D(texture, i_tex);

	float circle = 1.0 - length(uv - mouse) * 10;

	if (pixel.rgb == vec3(1.0))
	{
		FragColor = vec4(vec3(1.0), 0.0);
	}
	else
	{
		FragColor = pixel;
	}
};