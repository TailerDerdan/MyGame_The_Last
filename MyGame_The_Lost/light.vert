#version 130

varying vec2 i_tex;

void main() {
    vec4 texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    i_tex = texCoord.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}