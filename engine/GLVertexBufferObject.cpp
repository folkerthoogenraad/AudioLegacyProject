#include "GLVertexBufferObject.h"

#include "GL.h"

namespace apryx {

	GLVertexBufferObject::GLVertexBufferObject()
	{
		glGenBuffers(1, &m_ID);
	}

	GLVertexBufferObject::~GLVertexBufferObject()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void GLVertexBufferObject::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void GLVertexBufferObject::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBufferObject::setBufferData(DrawType type, size_t size, const void * data)
	{
		bind();

		int d = GL_STATIC_DRAW;
		if (type == DrawType::Stream)
			d = GL_STREAM_DRAW;
		if (type == DrawType::Dynamic)
			d = GL_DYNAMIC_DRAW;

		glBufferData(GL_ARRAY_BUFFER, size, data, d);
	}
	void GLVertexBufferObject::setBufferSubData(size_t offset, size_t size, const void * data)
	{
		bind();

		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
}
