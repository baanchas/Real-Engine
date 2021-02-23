#pragma once

#include "Layer.h"

namespace RealEngine {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Begin();
		void End();

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;
		void OnRender() override;
	private:
	};

}
