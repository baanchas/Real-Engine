#include "repch.h"
#include "RenderCommand.h"

namespace RealEngine {

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void RenderCommand::Enable(GLenum cap)
	{
		glEnable(cap);
	}

	void RenderCommand::SetBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		glBlendFunc(sfactor, dfactor);
	}

	void RenderCommand::SetDepthFunc(GLenum func)
	{
		glDepthFunc(func);
	}

	void RenderCommand::DrawTriangles(uint32_t count, GLenum type, const GLvoid* indices)
	{
		glDrawElements(GL_TRIANGLES, count, type, indices);
	}

}