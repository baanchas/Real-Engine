#pragma once

#include <iostream>

namespace RealEngine {

	class Application
	{
	public:
		
		~Application();

		void Run();
		void OnUpdate();
		void OnEvemt();
		void OnRender();

		void Exit();

		static Application* Get() { return s_Instance; }
	private:
		Application();
		static Application* s_Instance;

		bool m_Running;
	};

}
