#include "repch.h"

#include "Input.h"
#include "Application.h"


namespace RealEngine {

	Input* Input::s_Instance = new Input();

	bool Input::IsKeyPressed(int keycode)
	{
		//auto window = Application::Get()->GetWindow()->GetNativeWindow();
		return glfwGetKey(Application::Get().GetWindow().GetNativeWindow(), keycode);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state;
	}


	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = Application::Get().GetWindow().GetNativeWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		std::pair<float, float > mousePos = GetMousePosition();
		return mousePos.first;
	}

	float Input::GetMouseY()
	{
		std::pair<float, float > mousePos = GetMousePosition();
		return mousePos.second;
	}


}
