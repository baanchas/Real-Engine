#include "repch.h"

#include "Application.h"

namespace RealEngine {

	Application* Application::s_Instance = new Application();

	Application::Application()
	{
		m_Running = true;
		m_Window = new Window();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			OnRender();
			OnUpdate();
			OnEvent();
		}
	}

	void Application::OnEvent()
	{
		glfwPollEvents();
	}

	void Application::OnUpdate()
	{
		if (glfwWindowShouldClose(m_Window->GetNativeWindow()))
			m_Running = false;

		m_Window->OnUpdate();

		if (Input::IsKeyPressed(KeyCodes::LEFT))
		{
			ENGINE_INFO("A is pressed!");
		}
		
	}

	void Application::OnRender()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Application::Exit()
	{
		m_Running = false;
	}
}