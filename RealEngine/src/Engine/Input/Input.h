#pragma once

namespace RealEngine {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int keycode);
		static std::pair<float, float> GetMousePosition();

		static float GetMouseX();
		static float GetMouseY();


	private:
		static Input* s_Instance;
	};

}

