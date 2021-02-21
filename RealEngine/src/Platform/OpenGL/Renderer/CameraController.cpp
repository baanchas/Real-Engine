#include "repch.h"
#include "CameraController.h"

#include "Application.h"

namespace RealEngine {

	CameraController::CameraController(float width, float height)
        : m_AspectRatio(width / height), m_Height(height / 2), m_Camera(-m_Height * m_AspectRatio * m_ZoomLevel, m_Height * m_AspectRatio * m_ZoomLevel, -m_Height * m_ZoomLevel, m_Height * m_ZoomLevel)
	{
	}

	CameraController::~CameraController()
	{
	}

	void CameraController::OnUpdate()
	{
        if (Input::IsKeyPressed(KeyCodes::LEFT))
        {
            m_CameraPosition -= glm::vec3(m_CameraSpeed, 0.0f, 0.0f);
        }
        else if (Input::IsKeyPressed(KeyCodes::RIGHT))
        {
            m_CameraPosition += glm::vec3(m_CameraSpeed, 0.0f, 0.0f);
        }

        if (Input::IsKeyPressed(KeyCodes::DOWN))
        {
            m_CameraPosition -= glm::vec3(0.0f, m_CameraSpeed, 0.0f);
        }
        else if (Input::IsKeyPressed(KeyCodes::UP))
        {
            m_CameraPosition += glm::vec3(0.0f, m_CameraSpeed, 0.0f);
        }

        if (Input::IsKeyPressed(KeyCodes::HOME))
        {
            m_CameraRotation += m_CameraRotationSpeed;
        }
        else if (Input::IsKeyPressed(KeyCodes::END))
        {
            m_CameraRotation -= m_CameraRotationSpeed;
        }


        if (Application::Get().GetWindow().GetYOffset() == 1)
        {
            m_ZoomLevel *= 0.95f;
            m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel * m_Height, m_AspectRatio * m_ZoomLevel * m_Height, -m_ZoomLevel * m_Height, m_ZoomLevel * m_Height);
        }
        else if (Application::Get().GetWindow().GetYOffset() == -1)
        {
            m_ZoomLevel *= 1.05f;

            m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel * m_Height, m_AspectRatio * m_ZoomLevel * m_Height, -m_ZoomLevel * m_Height, m_ZoomLevel * m_Height);
        }

        m_Camera.SetRotation(m_CameraRotation);
        m_Camera.SetPosition(m_CameraPosition);
	}

}
