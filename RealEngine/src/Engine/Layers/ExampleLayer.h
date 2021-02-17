#pragma once

#include "Layer.h"

namespace RealEngine {

	class ExampleLayer : public Layer
	{
	public:
		ExampleLayer() {};
		~ExampleLayer() {};

		void OnUpdate() override {};
		void OnEvent() override {};
		void OnRender() override {};
	private:
	};

}

