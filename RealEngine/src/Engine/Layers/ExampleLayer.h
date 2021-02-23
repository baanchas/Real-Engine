#pragma once

#include "Layer.h"

namespace RealEngine {

	class ExampleLayer : public Layer
	{
	public:
		ExampleLayer() {};
		~ExampleLayer() {};

		void OnUpdate(float ts) override {};
		void OnEvent(Event& event) override {};
		void OnRender() override {};
	private:
	};

}

