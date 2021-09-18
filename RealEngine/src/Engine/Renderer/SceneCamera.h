#pragma once

#include "Camera.h"
#include <glm/gtc/type_precision.hpp>

namespace RealEngine {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();

		virtual ~SceneCamera() {};

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;

		void Move(glm::mat4& transform) override;
		void Move(glm::vec3& position) override;
		void Rotate(glm::vec3& rotation) override;

		glm::vec3 GetDirection() { return m_Direction; }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }


		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }

		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }
		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		
	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 10000.0f;

		float m_AspectRatio = 0.0f;

		float m_Pitch = 0.0f, m_Yaw = -90.0f;

		glm::vec2 m_InitialMousePosition = { 620.0f, 360.0f };

		glm::vec3 m_Direction = { 0.0f, 0.0f, 0.0f };

		bool m_IsInitialized = true;

	};

}

