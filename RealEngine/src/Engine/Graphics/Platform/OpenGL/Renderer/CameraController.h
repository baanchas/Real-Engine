#pragma once

#include "OrthographicCamera.h"
#include "Engine/Events/Event.h"

namespace RealEngine {

	class CameraController
	{
	public:
		CameraController(float wdith, float height);
		~CameraController();

		void OnUpdate(float ts);
		
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_Camera; }

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		float m_Height;

		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraSpeed = 300.0f;
		float m_CameraRotationSpeed = 1.0f;

	};

}

