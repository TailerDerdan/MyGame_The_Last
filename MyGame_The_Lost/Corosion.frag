#version 120

uniform sampler2D screenTexture;
uniform sampler2D erosion;
uniform float erosion_max_offset;

uniform vec2 firstPoint;
uniform vec2 secondPoint;

out vec4 FragColor;

void main() {
	vec2 uv = gl_TexCoord[0].xy;
	
	vec4 screen_tex = texture2D(screenTexture, uv);
	vec4 erosion_tex = texture2D(erosion, uv);
	
	float erosion_threshold = distance(firstPoint, secondPoint) * erosion_max_offset * 2;
	float alpha = erosion_tex.r > erosion_threshold ? 0.0 : 1.0;
	FragColor = alpha > 0.0 ? screen_tex : vec4(0.0, 0.0, 0.0, 1.0);
}