#include "repch.h"
#include "SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace RealEngine {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::OnUpdate(float ts)
	{

		if (m_IsInitialized)
		{
			m_InitialMousePosition.x = Input::GetMouseX();
			m_InitialMousePosition.y = Input::GetMouseY();
			m_IsInitialized = false;
		}

		GLfloat xoffset = Input::GetMouseX() - m_InitialMousePosition.x;
		GLfloat yoffset = m_InitialMousePosition.y - Input::GetMouseY();  // Reversed since y-coordinates go from bottom to left
		m_InitialMousePosition.x = Input::GetMouseX();
		m_InitialMousePosition.y = Input::GetMouseY();

		GLfloat sensitivity = 0.05;	// Change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Direction = glm::normalize(front);

	}

	void SceneCamera::OnEvent(Event& event)
	{
		

	}

	void SceneCamera::Move(glm::mat4& transform)
	{
		if (Input::IsKeyPressed(KeyCodes::UP))
		{
			transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -0.1f));
		}
		else if (Input::IsKeyPressed(KeyCodes::DOWN))
		{
			transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.1f));
		}

		if (Input::IsKeyPressed(KeyCodes::LEFT))
		{
			transform = glm::translate(transform, glm::vec3(-0.1f, 0.0f, 0.0f));
		}
		else if (Input::IsKeyPressed(KeyCodes::RIGHT))
		{
			transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f));
		}
	}

	void SceneCamera::Move(glm::vec3& position)
	{
		if (Input::IsKeyPressed(KeyCodes::UP))
		{
			position += glm::vec3(0.0f, 0.0f, 0.1f);;
		}
		else if (Input::IsKeyPressed(KeyCodes::DOWN))
		{
			position += glm::vec3(0.0f, 0.0f, -0.1f);;
		}

		if (Input::IsKeyPressed(KeyCodes::LEFT))
		{
			position += glm::vec3(-0.1f, 0.0f, 0.0f);
		}
		else if (Input::IsKeyPressed(KeyCodes::RIGHT))
		{
			position += glm::vec3(0.1f, 0.0f, 0.0f);
		}
	}

	void SceneCamera::Rotate(glm::vec3& rotation)
	{
		rotation = m_Direction;
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = SceneCamera::ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = SceneCamera::ProjectionType::Perspective;

		m_PerspectiveFOV = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
		
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, -m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = +m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = +m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}

	}


}