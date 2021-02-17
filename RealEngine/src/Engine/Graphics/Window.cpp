#include "repch.h"
#include "Window.h"

namespace RealEngine {

	Window::Window()
	{
		Init(props);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::Init(const WindowProps& props)
	{
		m_Properties.Title = props.Title;
		m_Properties.Width = props.Width;
		m_Properties.Height = props.Height;

		if (!glfwInit())
			std::cout << "GLFW Error!" << std::endl;

		m_Window = glfwCreateWindow(m_Properties.Width, m_Properties.Height, m_Properties.Title.c_str(), NULL, NULL);

		/* Make the window's context current */
		glfwMakeContextCurrent(m_Window);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "glew Init Error!" << std::endl;
		}

		glfwSetWindowUserPointer(m_Window, &m_Properties);

		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;
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
