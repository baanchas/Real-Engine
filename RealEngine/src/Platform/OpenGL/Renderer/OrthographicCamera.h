#pragma once

namespace RealEngine {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);


		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); };
		
		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; CalculateViewMatrix(); };

		void SetProjection(float left, float right, float bottom, float top);
		
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetPViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void CalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;

		float m_CenterX;
		float m_CenterY;

		float m_Rotation = 0.0f;

	};

}

