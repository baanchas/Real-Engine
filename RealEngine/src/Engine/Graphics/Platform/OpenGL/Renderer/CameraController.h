#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"
#include "Engine/Events/Event.h"

namespace RealEngine {

	class CameraController
	{
	public:
		CameraController(float wdith, float height);
		~CameraController();

		void OnUpdate(float ts);
		
		void OnEvent(Event& event);

		void OnBoundsResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_OrthographicCamera; }

	private:
		float m_ZoomLevel = 1.0f;

		float m_ProjectionWidth;
		float m_ProjectionHeight;

		OrthographicCamera m_OrthographicCamera;
		//Camera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraSpeed = 300.0f;
		float m_CameraRotationSpeed = 1.0f;

	};

}

