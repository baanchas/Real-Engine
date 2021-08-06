#pragma once

namespace RealEngine {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		glm::vec3 Albedo;
		float Metallic;
		float Roughness;
		float AO;
		float TexId;
		float TilingFactor;
		int entityID;

		//glm::vec3 Ambient;
		//glm::vec3 Diffuse;
		//glm::vec3 Specular;

	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void Create(uint32_t size);

		void Bind();
		void UnBind();

		void SetData(const void* data, uint32_t size);

	private:
		unsigned int m_RendererID;
	};

}

