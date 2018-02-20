#include "GLTexture.h"

#include "GL.h"

namespace apryx {
	GLTexture::GLTexture()
	{
		glGenTextures(1, &m_ID);
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void GLTexture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
	void GLTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::setFiltering(TextureFiltering filtering)
	{
		GLuint f = filtering == TextureFiltering::Linear ? GL_LINEAR : GL_NEAREST;

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
	}

	void GLTexture::setWrapping(WrappingMode wrapping)
	{
		GLuint f = wrapping == WrappingMode::Repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
	}

	void GLTexture::setData(const Image & image)
	{
		bind();
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.getColors()[0]);
	}
}
