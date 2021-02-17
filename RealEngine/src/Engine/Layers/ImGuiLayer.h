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

		void OnUpdate() override;
		void OnEvent() override;
		void OnRender() override;
	private:
	};

}
