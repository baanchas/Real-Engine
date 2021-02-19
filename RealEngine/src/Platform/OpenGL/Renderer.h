#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


namespace RealEngine {

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() {};

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT); };
		static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	private:
	};

}