#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;
in float v_TilingFactor;
in flat int v_EntityID;
in vec3 v_Normal;
in vec3 FragPos;

uniform sampler2D u_Texture[32];
void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

	float distance = length(lightPos- FragPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance +
		0.032f * (distance * distance));

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * vec3(1.0f, 1.0f, 1.0f);
	vec4 texColor = v_Color;


	switch (int(v_TexID))
	{
	case  0: texColor *= texture(u_Texture[0], v_TexCoord * v_TilingFactor); break;
	case  1: texColor *= texture(u_Texture[1], v_TexCoord * v_TilingFactor); break;
	case  2: texColor *= texture(u_Texture[2], v_TexCoord * v_TilingFactor); break;
	case  3: texColor *= texture(u_Texture[3], v_TexCoord * v_TilingFactor); break;
	case  4: texColor *= texture(u_Texture[4], v_TexCoord * v_TilingFactor); break;
	case  5: texColor *= texture(u_Texture[5], v_TexCoord * v_TilingFactor); break;
	case  6: texColor *= texture(u_Texture[6], v_TexCoord * v_TilingFactor); break;
	case  7: texColor *= texture(u_Texture[7], v_TexCoord * v_TilingFactor); break;
	case  8: texColor *= texture(u_Texture[8], v_TexCoord * v_TilingFactor); break;
	case  9: texColor *= texture(u_Texture[9], v_TexCoord * v_TilingFactor); break;
	case 10: texColor *= texture(u_Texture[10], v_TexCoord * v_TilingFactor); break;
	case 11: texColor *= texture(u_Texture[11], v_TexCoord * v_TilingFactor); break;
	case 12: texColor *= texture(u_Texture[12], v_TexCoord * v_TilingFactor); break;
	case 13: texColor *= texture(u_Texture[13], v_TexCoord * v_TilingFactor); break;
	case 14: texColor *= texture(u_Texture[14], v_TexCoord * v_TilingFactor); break;
	case 15: texColor *= texture(u_Texture[15], v_TexCoord * v_TilingFactor); break;
	case 16: texColor *= texture(u_Texture[16], v_TexCoord * v_TilingFactor); break;
	case 17: texColor *= texture(u_Texture[17], v_TexCoord * v_TilingFactor); break;
	case 18: texColor *= texture(u_Texture[18], v_TexCoord * v_TilingFactor); break;
	case 19: texColor *= texture(u_Texture[19], v_TexCoord * v_TilingFactor); break;
	case 20: texColor *= texture(u_Texture[20], v_TexCoord * v_TilingFactor); break;
	case 21: texColor *= texture(u_Texture[21], v_TexCoord * v_TilingFactor); break;
	case 22: texColor *= texture(u_Texture[22], v_TexCoord * v_TilingFactor); break;
	case 23: texColor *= texture(u_Texture[23], v_TexCoord * v_TilingFactor); break;
	case 24: texColor *= texture(u_Texture[24], v_TexCoord * v_TilingFactor); break;
	case 25: texColor *= texture(u_Texture[25], v_TexCoord * v_TilingFactor); break;
	case 26: texColor *= texture(u_Texture[26], v_TexCoord * v_TilingFactor); break;
	case 27: texColor *= texture(u_Texture[27], v_TexCoord * v_TilingFactor); break;
	case 28: texColor *= texture(u_Texture[28], v_TexCoord * v_TilingFactor); break;
	case 29: texColor *= texture(u_Texture[29], v_TexCoord * v_TilingFactor); break;
	case 30: texColor *= texture(u_Texture[30], v_TexCoord * v_TilingFactor); break;
	case 31: texColor *= texture(u_Texture[31], v_TexCoord * v_TilingFactor); break;
	}

	//ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (ambient + diffuse + specular) * vec3(texColor[0], texColor[1], texColor[2]);
	//vec3 result = { ambientStrength * texColor[0], ambientStrength * texColor[1], ambientStrength * texColor[2] };
	color = vec4(result, 1.0f);
	//color = texColor;
	color2 = v_EntityID;
}