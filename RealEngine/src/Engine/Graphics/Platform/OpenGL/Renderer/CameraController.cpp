#include "repch.h"
#include "CameraController.h"

#include "Application.h"


namespace RealEngine {

	CameraController::CameraController(float width, float height)
        : m_ProjectionWidth(width / 2), m_ProjectionHeight(height / 2), m_Camera(-m_ProjectionWidth * m_ZoomLevel, m_ProjectionWidth * m_ZoomLevel, -m_ProjectionHeight * m_ZoomLevel, m_ProjectionHeight * m_ZoomLevel)
	{
	}

	CameraController::~CameraController()
	{
	}

    void CameraController::OnEvent(Event& event)
    {
        /*if (event.Type == EventType::WindowResized)
        {
            float aspectRatio;

            if (1280.0f / event.WindowResized.Width > 720.0f / event.WindowResized.Height)
            {
                aspectRatio = 1280.0f / event.WindowResized.Width;
            }
            else
            {
                aspectRatio = 720.0f / event.WindowResized.Height;
            }

            m_ProjectionWidth = aspectRatio * (float)event.WindowResized.Width / 2;
            m_ProjectionHeight = aspectRatio * (float)event.WindowResized.Height / 2;

            m_Camera.SetProjection(-m_ProjectionWidth * m_ZoomLevel, m_ProjectionWidth  * m_ZoomLevel, -m_ProjectionHeight  * m_ZoomLevel, m_ProjectionHeight * m_ZoomLevel);
        }*/

        if (event.Type == EventType::MouseScrolled)
        {
            if (event.MouseScrolled.yOffset == 1)
            {
                m_ZoomLevel *= 0.95f;
                m_Camera.SetProjection(-m_ProjectionWidth * m_ZoomLevel, m_ProjectionWidth * m_ZoomLevel, -m_ProjectionHeight * m_ZoomLevel, m_ProjectionHeight * m_ZoomLevel);

                if (m_CameraSpeed > 20.0f)
                    m_CameraSpeed -= 10.0f; 
            }
            else if (event.MouseScrolled.yOffset == -1)
            {
                m_ZoomLevel *= 1.05f;

                m_Camera.SetProjection(-m_ProjectionWidth * m_ZoomLevel, m_ProjectionWidth * m_ZoomLevel, -m_ProjectionHeight * m_ZoomLevel, m_ProjectionHeight * m_ZoomLevel);


                m_CameraSpeed += 10.0f;
            }
        }
        
    }

	void CameraController::OnUpdate(float ts)
	{
        if (Input::IsKeyPressed(KeyCodes::LEFT))
        {
            m_CameraPosition -= glm::vec3(m_CameraSpeed, 0.0f, 0.0f) * ts;
        }
        else if (Input::IsKeyPressed(KeyCodes::RIGHT))
        {
            m_CameraPosition += glm::vec3(m_CameraSpeed, 0.0f, 0.0f) * ts;
        }

        if (Input::IsKeyPressed(KeyCodes::DOWN))
        {
            m_CameraPosition -= glm::vec3(0.0f, m_CameraSpeed, 0.0f) * ts;
        }
        else if (Input::IsKeyPressed(KeyCodes::UP))
        {
            m_CameraPosition += glm::vec3(0.0f, m_CameraSpeed, 0.0f) * ts;
        }

        if (Input::IsKeyPressed(KeyCodes::HOME))
        {
            m_CameraRotation += m_CameraRotationSpeed;
        }
        else if (Input::IsKeyPressed(KeyCodes::END))
        {
            m_CameraRotation -= m_CameraRotationSpeed;
        }


        m_Camera.SetRotation(m_CameraRotation);
        m_Camera.SetPosition(m_CameraPosition);
	}

    void CameraController::OnBoundsResize(float width, float height)
    {
        float aspectRatio;

        if (1280.0f / width > 720.0f / height)
        {
            aspectRatio = 1280.0f / width;
        }
        else
        {
            aspectRatio = 720.0f / height;
        }

        m_ProjectionWidth = aspectRatio * (float)width / 2;
        m_ProjectionHeight = aspectRatio * (float)height / 2;

        m_Camera.SetProjection(-m_ProjectionWidth * m_ZoomLevel, m_ProjectionWidth * m_ZoomLevel, -m_ProjectionHeight * m_ZoomLevel, m_ProjectionHeight * m_ZoomLevel);
    }

}
