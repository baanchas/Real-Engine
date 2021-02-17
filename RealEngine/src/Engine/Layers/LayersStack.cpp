#include "repch.h"
#include "LayersStack.h"


namespace RealEngine {

	LayersStack::LayersStack()
	{
	}

	LayersStack::~LayersStack()
	{
	}

	void LayersStack::PushLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
	}

	void LayersStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end() - 1, layer);
		m_Layers.erase(it);
	}
}