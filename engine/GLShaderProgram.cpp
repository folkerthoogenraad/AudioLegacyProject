#include "GLShaderProgram.h"

#include "math/Vector3.h"
#include "math/Matrix4.h"

#include <iostream>
#include <assert.h>
#include "GL.h"

namespace apryx {
	GLShaderProgram::GLShaderProgram(const GLShader &vertexShader, const GLShader &fragmentShader)
	{
		assert(vertexShader.getType() == GLShader::Vertex);
		assert(fragmentShader.getType() == GLShader::Fragment);

		m_ID = glCreateProgram();

		glAttachShader(m_ID, vertexShader.getID());
		glAttachShader(m_ID, fragmentShader.getID());

		glBindFragDataLocation(m_ID, 0, "outColor");

		glBindAttribLocation(m_ID, SHADER_POSITION_LOCATION, SHADER_POSITION_NAME);
		glBindAttribLocation(m_ID, SHADER_UV_LOCATION, SHADER_UV_NAME);
		glBindAttribLocation(m_ID, SHADER_COLOR_LOCATION, SHADER_COLOR_NAME);

		glLinkProgram(m_ID);

		checkGLError();
	}

	void GLShaderProgram::use()
	{
		glUseProgram(m_ID);
	}

	void GLShaderProgram::reset()
	{
		glUseProgram(0);
	}

	int GLShaderProgram::getAttribLocation(const char * name)
	{
		return glGetAttribLocation(m_ID, name);
	}
	int GLShaderProgram::getUniformLocation(const char * name)
	{
		return glGetUniformLocation(m_ID, name);
	}

	void GLShaderProgram::setUniform(int index, float v)
	{
		glUniform1f(index, v);
	}

	void GLShaderProgram::setUniform(int index, Vector3f v)
	{
		glUniform3f(index, v.x, v.y, v.z);
	}

	void GLShaderProgram::setUniform(int index, Matrix4f v)
	{
		glUniformMatrix4fv(index, 1, GL_TRUE, &v.a);
	}
}
