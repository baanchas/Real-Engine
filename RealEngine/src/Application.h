#pragma once

#include <iostream>
#include "Engine/Graphics/Window.h"

namespace RealEngine {

	class Application
	{
	public:
		
		~Application();

		void Run();
		void OnUpdate();
		void OnEvent();
		void OnRender();

		void Exit();

		inline Window* GetWindow() { return m_Window; }

		static Application* Get() { return s_Instance; }
	private:
		Application();
		static Application* s_Instance;
		Window* m_Window;
		bool m_Running;
	};

}
