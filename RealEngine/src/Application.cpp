#include "Application.h"

namespace RealEngine {

	Application* Application::s_Instance = new Application();

	Application::Application()
	{
		m_Running = true;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			OnEvemt();
			OnUpdate();
			OnRender();
		}
	}

	void Application::OnEvemt()
	{
	
	}

	void Application::OnUpdate()
	{
		std::cout << "Welcome to Engine" << std::endl;
	}

	void Application::OnRender()
	{
	}

	void Application::Exit()
	{
		m_Running = false;
	}
}