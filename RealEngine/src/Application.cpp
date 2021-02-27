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

		PushLayer(new EditorLayer());
		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);
	}

	Application::~Application()
	{
		delete m_Window;
		delete m_ImGuiLayer;
		delete s_Instance;
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

		m_Event = m_Window->GetEvent();

		for (Layer* layer : m_LayerStack)
		{
			layer->OnEvent(m_Event);
		}

	}

	void Application::OnUpdate()
	{
		if (glfwWindowShouldClose(m_Window->GetNativeWindow()))
			m_Running = false;

		float time = (float)glfwGetTime();
		m_TimeStep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		m_ImGuiLayer->Begin();

		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate(m_TimeStep);
		}
		
		m_Window->OnUpdate();
	}

	void Application::OnRender()
	{
		Renderer::Clear();

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