#pragma once

#include "Camera.h"


namespace RealEngine {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() {};

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void RecalculateProjection();

	private:
		float m_Size = 10.0f;
		float m_Near = -1.0f;
		float m_Far = 1.0f;

		float m_AspectRatio = 0.0f;

	};

}

