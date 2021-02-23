#include "repch.h"
#include "Window.h"

namespace RealEngine {

	Window::Window()
	{
		Init(props);
	}

	Window::~Window()
	{
		delete m_Window;
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		//glfwSwapBuffers(m_Window);
		m_Properties.m_Event = Event();
	}

	void Window::Init(const WindowProps& props)
	{
		m_Properties.Title = props.Title;
		m_Properties.Width = props.Width;
		m_Properties.Height = props.Height;

		if (!glfwInit())
			std::cout << "GLFW Error!" << std::endl;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Properties.Width, m_Properties.Height, m_Properties.Title.c_str(), NULL, NULL);

		/* Make the window's context current */
		glfwMakeContextCurrent(m_Window);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "glew Init Error!" << std::endl;
		}


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetWindowUserPointer(m_Window, &m_Properties);

		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			Event event;
			event.Type = EventType::WindowResized;
			event.WindowResized.Width = width;
			event.WindowResized.Height = height;

			data.m_Event = event;

		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			Event event;
			event.Type = EventType::MouseScrolled;

			event.MouseScrolled.yOffset = yOffset;
			event.MouseScrolled.xOffset = xOffset;

			data.m_Event = event;

		});

		//SetVSync(props.VSync);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(2);

		m_Properties.VSync = enabled;
	}
}
