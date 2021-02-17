#pragma once

namespace RealEngine {

	class Layer
	{
	public:
		Layer() {};
		virtual ~Layer() {};

		virtual void OnUpdate() {};
		virtual void OnRender() {};
		virtual void OnEvent() {};
	private:
		
	};

}

