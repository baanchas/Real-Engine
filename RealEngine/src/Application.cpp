#include "repch.h"

#include "Application.h"


namespace RealEngine {

	Application* Application::s_Instance = new Application();

	Application::Application()
	{
		s_Instance = this;


		RealEngine::Log::InitLog();

		m_Running = true;
		m_Window = new Window();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		PushLayer(new EditorLayer());
		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			OnEvent();
			OnUpdate();
			OnRender();
			glfwSwapBuffers(m_Window->GetNativeWindow());
		}
	}

	void Application::OnEvent()
	{
		glfwPollEvents();

		for (Layer* layer : m_LayerStack)
		{
			layer->OnEvent();
		}
	}

	void Application::OnUpdate()
	{
		if (glfwWindowShouldClose(m_Window->GetNativeWindow()))
			m_Running = false;

		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate();
		}
		
		if (Input::IsKeyPressed(KeyCodes::LEFT))
		{
			ENGINE_INFO("A is pressed!");
		}
		
		//m_Window->OnUpdate();
	}

	void Application::OnRender()
	{
		Renderer::Clear();
		//glClear(GL_COLOR_BUFFER_BIT);

		m_ImGuiLayer->Begin();

		for (Layer* layer : m_LayerStack)
		{
			layer->OnRender();
		}

		m_ImGuiLayer->End();
	}

	void Application::Exit()
	{
		m_Running = false;
	}
}