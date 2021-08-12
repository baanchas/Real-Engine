#version 330 core

layout(location = 0) out vec4 Color;
layout(location = 1) out int ID;

const float PI = 3.14159265359;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Albedo;
in float v_FlatColor;
in float v_Metallic;
in float v_Roughness;
in float v_AO;
in float v_TexID;
in float v_TilingFactor;
in flat int v_EntityID;
in mat4 v_MeshTransform;

uniform sampler2D u_Texture[27];
uniform sampler2D u_TexturedMap[5];
uniform int u_IsTextured;
uniform int u_EntityID;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_TexturedMap[4], v_TexCoord).xyz * 2.0 - 1.0; //normal map

    vec3 Q1  = dFdx(v_Position);
    vec3 Q2  = dFdy(v_Position);
    vec2 st1 = dFdx(v_TexCoord);
    vec2 st2 = dFdy(v_TexCoord);

    vec3 N   = normalize(v_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return TBN * tangentNormal;
}

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
	vec4 texColor;
	int id;

	if (v_FlatColor == 1.0)
	{
		texColor = v_Color;
		id = v_EntityID;

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
		}
	}
	else
	{
		vec3 albedo = v_Albedo;
		vec3 normal = v_Normal;
		float metallic = v_Metallic;
		float roughness = v_Roughness;
		float ao = v_AO;

		if (u_IsTextured == 1)
		{
			albedo = pow(texture(u_TexturedMap[0], v_TexCoord).rgb, vec3(2.2)); //albedo
			metallic = texture(u_TexturedMap[1], v_TexCoord).r; // metallic
			roughness = texture(u_TexturedMap[2], v_TexCoord).r; // roughness
			ao = texture(u_TexturedMap[3], v_TexCoord).r; // AO

			normal = getNormalFromMap();
		}

		vec4 posV = v_MeshTransform * vec4(v_Position, 1.0f);
		vec3 N = normalize(normal);
		vec3 V = normalize(viewPos - vec3(posV[0], posV[1], posV[2]));

		vec3 F0 = vec3(0.04);
		F0 = mix(F0, albedo, metallic);

		vec3 Lo = vec3(0.0);

		vec3 L = normalize(lightPos - vec3(posV[0], posV[1], posV[2]));
		vec3 H = normalize(V + L);
		float distance = length(lightPos - vec3(posV[0], posV[1], posV[2]));
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = vec3(300.0f, 300.0f, 300.0f) * attenuation;

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = numerator / max(denominator, 0.001);

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;

		vec3 ambient = vec3(0.03) * albedo * ao * attenuation;
		vec3 colour = ambient + Lo;

		colour = colour / (colour + vec3(1.0));
		colour = pow(colour, vec3(1.0 / 2.2));
		texColor = vec4(colour, 1.0);
		id = u_EntityID;
	}

	Color = texColor;
	ID = id;
}