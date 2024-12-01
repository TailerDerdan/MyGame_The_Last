#version 330 core

precision mediump float;

layout(location = 0) out vec4 color;

in vec2 i_tex;

uniform sampler2D texture;

uniform vec2 u_mouse;
uniform vec2 u_resolution;

//layout(std140) uniform Array {
//	bool mapOfWall[30800];
//};

void main() {
    vec2 uv	   = gl_FragCoord.xy / u_resolution;
	vec2 mouse = u_mouse		 / u_resolution;

	mouse.y    = 1.0 - mouse.y;

	vec4 pixel = texture2D(texture, i_tex);

	float circle = 1.0 - length(uv - mouse) * 20.0;

	int xCoord = int(floor(uv.x / 200));
	int yCoord = int(floor(uv.y / 154));

	//if (!mapOfWall[xCoord + yCoord * 200])
	//{
		//color = vec4(vec3(pixel.xyz), circle * (-1.0 + pixel.b) * 1.35);
	//}
	//else
	//{
		//color = vec4(vec3(0.1), 1.0);
	//}
}