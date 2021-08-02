#include "repch.h"
#include "Window.h"
#include "Application.h"

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
		glfwWindowHint(GLFW_SAMPLES, 4);

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

			Event event;
			event.Type = EventType::WindowResized;
			event.WindowResized.Width = width;
			event.WindowResized.Height = height;

			Application::Get().OnEvent(event);

		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
			
			Event event;

			if (action == GLFW_PRESS)
			{
				event.Type = EventType::KeyPressed;

				event.KeyPressed.Key = key;
				event.KeyPressed.ScanCode = scancode;
				event.KeyPressed.Action = action;
				event.KeyPressed.Mods = mods;
			}
			else if (action == GLFW_RELEASE)
			{
				event.Type = EventType::KeyReleased;

				event.KeyReleased.Key = key;
				event.KeyReleased.ScanCode = scancode;
				event.KeyReleased.Action = action;
				event.KeyReleased.Mods = mods;
			}
			else if (action == GLFW_REPEAT)
			{
				event.Type = EventType::KeyRepeated;

				event.KeyReleased.Key = key;
				event.KeyReleased.ScanCode = scancode;
				event.KeyReleased.Action = action;
				event.KeyReleased.Mods = mods;
			}

			Application::Get().OnEvent(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			Event event;

			if (action == GLFW_PRESS)
			{
				event.Type = EventType::MouseButtonPressed;

				event.MouseButtonPressed.Button = button;
				event.MouseButtonPressed.Action = action;
				event.MouseButtonPressed.Mods = mods;
			}
			else if (action == GLFW_RELEASE)
			{
				event.Type = EventType::MouseButtonReleased;

				event.MouseButtonReleased.Button = button;
				event.MouseButtonReleased.Action = action;
				event.MouseButtonReleased.Mods = mods;
			}

			Application::Get().OnEvent(event);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);

			Event event;
			event.Type = EventType::MouseScrolled;

			event.MouseScrolled.yOffset = yOffset;
			event.MouseScrolled.xOffset = xOffset;

			Application::Get().OnEvent(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowProperties& data = *(WindowProperties*)glfwGetWindowUserPointer(window);
			
			Event event;
			event.Type = EventType::MouseMoved;

			event.MouseMoved.xOffset = xPos;
			event.MouseMoved.yOffset = yPos;

			Application::Get().OnEvent(event);
		});

		//SetVSync(props.VSync);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Properties.VSync = enabled;
	}
}
