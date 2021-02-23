#pragma once

#include "Engine/Events/Event.h"

namespace RealEngine {

	class Layer
	{
	public:
		Layer() {};
		virtual ~Layer() {};

		virtual void OnUpdate(float ts) {};
		virtual void OnRender() {};
		virtual void OnEvent(Event& event) {};
	private:
		
	};

}

