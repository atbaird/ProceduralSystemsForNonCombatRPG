#version 410 core //necessary for all shaders

uniform sampler2D gDiffuseTex;
uniform int gColorChoice;
in vec2 passUV;
in vec4 passColor;

out vec4 outColor;

vec4 UVtoColor(vec2 uv)
{
	return vec4(uv, 0.0f, 1.0f);
}

float GetLuminance(vec4 val)
{
	vec3 luminanceVals = vec3(0.21, 0.71, 0.07);
	return dot(val.rgb, luminanceVals);
}

void main(void)
{
	int maxColChoice = 6;

	vec4 c = texture(gDiffuseTex, passUV);
	float luminance = GetLuminance(c);
	vec3 vec3EndCol = c.rgb * (passColor.rgb * luminance);
	vec4 endCol = vec4(vec3EndCol.xyz, passColor.a * c.a);

	if(gColorChoice <= 0 || gColorChoice > maxColChoice)
	{
		//Normal colors
		outColor = c * passColor;
	}
	else if(gColorChoice == 1)
	{
		//RGB
		outColor = vec4(endCol.r, endCol.g, endCol.b, endCol.a);
	}
	else if(gColorChoice == 2)
	{
		//RBG
		outColor = vec4(endCol.r, endCol.b, endCol.g, endCol.a);
	}
	else if(gColorChoice == 3)
	{
		//BGR
		outColor = vec4(endCol.b, endCol.g, endCol.r, endCol.a);
	}
	else if(gColorChoice == 4)
	{
		//BRG
		outColor = vec4(endCol.r, endCol.g, endCol.b, endCol.a);
	}
	else if(gColorChoice == 5)
	{
		//GRB
		outColor = vec4(endCol.g, endCol.r, endCol.b, endCol.a);
	}
	else if(gColorChoice == maxColChoice)
	{
		//GBR
		outColor = vec4(endCol.g, endCol.b, endCol.r, endCol.a);
	}
	/*
	Other selection possibilities:
		RRR		RRB		BBR		GGR		RRG		BBG		GGB
		GGG		RBR		BRB		GRG		RGR		BGB		GBG
		BBB		BRR		RBB		RGG		GRR		GBB		BGG
	*/
}