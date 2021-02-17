#pragma once

#include "Engine/Layer.h"

namespace RealEngine {

	class ExampleLayer : public Layer
	{
	public:
		ExampleLayer() { ENGINE_INFO("ExampleLayer is pushed"); };
		~ExampleLayer() {};

		void OnUpdate() override { ENGINE_INFO("updating examplelayer"); };
		void OnEvent() override {};
		void OnRender() override {};
	private:
	};

}

