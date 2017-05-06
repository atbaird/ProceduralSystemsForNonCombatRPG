#version 410 core //necessary for all shaders

uniform sampler2D gDiffuseTex;
in vec2 passUV;
in vec4 passColor;

out vec4 outColor;

void main(void)
{
	vec4 c = texture(gDiffuseTex, passUV);
	outColor = c * passColor;
}