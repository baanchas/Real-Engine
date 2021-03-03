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
			glfwPollEvents();

			OnUpdate();
			OnRender();

			glfwSwapBuffers(m_Window->GetNativeWindow());
		}
	}

	void Application::OnEvent(Event& event)
	{
		if (event.Type == EventType::WindowResized)
		{
			if (event.WindowResized.Width == 0 || event.WindowResized.Height == 0)
			{
				m_Minimizied = true;
			}
			else
			{
				m_Minimizied = false;

				Renderer::SetViewport(0, 0, event.WindowResized.Width, event.WindowResized.Height);
			}
		}

		for (Layer* layer : m_LayerStack)
		{
			layer->OnEvent(event);
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

		if (!m_Minimizied)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(m_TimeStep);
			}
		}
	}

	void Application::OnRender()
	{

		if (!m_Minimizied)
		{
			Renderer::Clear();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnRender();
			}
		}

		for (Layer* layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}

		m_ImGuiLayer->End();
	}

	void Application::Exit()
	{
		m_Running = false;
	}
}