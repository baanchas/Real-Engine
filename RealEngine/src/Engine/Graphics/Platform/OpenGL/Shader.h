#pragma once


namespace RealEngine {

	class Shader
	{
	public:
		Shader();
		~Shader();


		void Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		void Bind() const;
		void UnBind() const;

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformVec4Float(const std::string& name, float v0, float v1, float v2, float v3);
		void UploadUniformVec3Float(const std::string& name, float v0, float v1, float v2);

		void UploadUniformIntArray(const std::string& name, int* value, uint32_t count);
		void UploadUniformFloatArray(const std::string& name, const float* value, uint32_t count);
		void UploadUniformVec3Array(const std::string& name, const glm::vec3* value, uint32_t count);

		void UploadUniformMat4Float(const std::string& name, const glm::mat4& matrix);

		int GetUniformLocation(const std::string& uniform);

	private:
		uint32_t m_RendererID;
		int m_UniformLocation;

		std::string m_VertexShader;
		std::string m_FragmentShader;
	};

}