#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

uniform vec3 lightPos;
uniform vec3 viewPos;


in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;
in float v_TilingFactor;
in flat int v_EntityID;
in vec3 v_Normal;
in vec3 FragPos;
in vec3 v_Albedo;
in float v_Metallic;
in float v_Roughness;
in float v_AO;

uniform sampler2D u_Texture[32];

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	/*float ambientStrength = 1.1f;
	vec3 ambient = vec3(1.0f, 1.0f, 1.0f) * ambientStrength * v_Ambient;

	float distance = length(lightPos- FragPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance +
		0.032f * (distance * distance));

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * (diff * v_Diffuse);
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_Shininness * 128.0);
	vec3 specular = vec3(1.0f, 1.0f, 1.0f) * (specularStrength * spec * v_Specular);*/

	vec3 N = normalize(v_Normal);
	vec3 V = normalize(viewPos - v_Position);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, v_Albedo, v_Metallic);

	vec3 Lo = vec3(0.0);

	vec3 L = normalize(lightPos - v_Position);
	vec3 H = normalize(V + L);
	float distance = length(lightPos - v_Position);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = vec3(300.0f, 300.0f, 300.0f) * attenuation;

	float NDF = DistributionGGX(N, H, v_Roughness);
	float G = GeometrySmith(N, V, L, v_Roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - v_Metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	float NdotL = max(dot(N, L), 0.0);
	Lo += (kD * v_Albedo / PI + specular) * radiance * NdotL;

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
//	diffuse *= attenuation;
	//specular *= attenuation;

	//vec3 result = (ambient + diffuse + specular) * vec3(texColor[0], texColor[1], texColor[2]);
	//color = vec4(result, 1.0f);

	vec3 ambient = vec3(0.03) * v_Albedo * v_AO;
	vec3 colour = ambient + Lo;

	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0 / 2.2));
	//colour = colour * vec3(texColor[0], texColor[1], texColor[2]);
	color = vec4(colour, 1.0);

	color2 = v_EntityID;
}