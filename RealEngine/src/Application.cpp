#include "repch.h"

#include "Application.h"


namespace RealEngine {

	Application* Application::s_Instance = new Application();

	Application::Application()
	{
		m_Running = true;
		m_Window = new Window();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Application::Exit()
	{
		m_Running = false;
	}
}