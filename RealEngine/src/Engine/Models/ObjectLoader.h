#pragma once

#include "OpenGL/VertexBuffer.h"

namespace RealEngine {

	class ObjectLoader
	{
	public:
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> LoadObjectFromOBJ(const std::string& file_name);

	private:
		
	};


}