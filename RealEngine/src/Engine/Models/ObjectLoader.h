#pragma once

#include "OpenGL/VertexBuffer.h"

namespace RealEngine {

	class ObjectLoader
	{
	public:
		static std::pair<std::vector<glm::vec3>, std::vector<uint32_t>> LoadObjectFromOBJ(const std::string& file_name);

	private:
		
	};


}