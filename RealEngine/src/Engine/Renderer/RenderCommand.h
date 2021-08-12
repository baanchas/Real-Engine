#pragma once

namespace RealEngine {

	class RenderCommand
	{
	public:
		static void Clear();
		static void Enable(GLenum cap);
		static void SetBlendFunc(GLenum sfactor, GLenum dfactor);
		static void SetDepthFunc(GLenum func);
		static void DrawElements(GLenum mode, uint32_t count, GLenum type, const GLvoid* indices);
	};

}