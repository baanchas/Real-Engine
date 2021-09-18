#pragma once 

#include <glm/glm.hpp>

namespace RealEngine {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() {};

		virtual void OnUpdate(float ts) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnRender() {};

		virtual void Move(glm::mat4& transform) {};
		virtual void Move(glm::vec3& position) {};
		virtual void Rotate(glm::vec3& rotation) {};

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::vec3 GetDirection() const { return m_Rotation; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	};

}