#version 410 core

uniform vec4 gColor; //constant color
uniform sampler2D gDiffuseTex;

in vec4 passColor;
in vec2 passUV0;

out vec4 outColor;

void main(void)
{
	vec4 diffuse = texture(gDiffuseTex, passUV0);
	outColor = vec4(1.f,0.f,0.f,1.f);//passColor * diffuse;
	// outColor = vec4(passUV0, 0.f, 1.f);

	//outColor = passColor; //passthrough only.
}