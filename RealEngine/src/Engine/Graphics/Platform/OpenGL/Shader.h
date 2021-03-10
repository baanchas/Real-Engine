#pragma once


namespace RealEngine {

	class Shader
	{
	public:
		Shader();
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniformIntArray(const std::string& name, int* value, uint32_t count);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);


		int GetUniformLocation(const std::string& uniform);

	private:
		uint32_t m_RendererID;
		int m_UniformLocation;
	};

}