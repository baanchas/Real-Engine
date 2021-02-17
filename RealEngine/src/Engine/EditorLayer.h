#pragma once

#include "Layer.h"

namespace RealEngine {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnUpdate() override;
		void OnEvent() override;
		void OnRender() override;
	private:
	};

}

