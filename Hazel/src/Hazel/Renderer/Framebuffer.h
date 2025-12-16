#pragma once

#include "../Core.h"

namespace Hazel {

	class HAZEL_API Framebuffer
	{
	public:
		Framebuffer(unsigned int width, unsigned int height);
		~Framebuffer();

		void Bind();
		void Unbind();

		void Resize(unsigned int width, unsigned int height);

		unsigned int GetColorAttachment() const { return m_ColorAttachment; }
		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

	private:
		unsigned int m_FramebufferID = 0;
		unsigned int m_ColorAttachment = 0;
		unsigned int m_DepthAttachment = 0;
		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
	};

}
