#pragma once

#include "Layer.h"

namespace RealEngine {

	class LayersStack
	{
	public:
		LayersStack();
		~LayersStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		inline Layer* top() { return m_Layers.back(); }
		inline int GetSize() const { return (int)m_Layers.size(); }

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
	};

}