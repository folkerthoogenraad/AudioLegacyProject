#include "ShaderProgram.h"

#include <iostream>
#include <assert.h>
#include "GL.h"

namespace apryx {
	ShaderProgram::ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader)
	{
		assert(vertexShader.getType() == Shader::Vertex);
		assert(fragmentShader.getType() == Shader::Fragment);

		m_ID = glCreateProgram();

		glAttachShader(m_ID, vertexShader.getID());
		glAttachShader(m_ID, fragmentShader.getID());

		glBindFragDataLocation(m_ID, 0, "color");

		glBindAttribLocation(m_ID, SHADER_POSITION_LOCATION, SHADER_POSITION_NAME);

		glLinkProgram(m_ID);

		checkGLError();
	}

	void ShaderProgram::use()
	{
		glUseProgram(m_ID);
	}

	void ShaderProgram::reset()
	{
		glUseProgram(0);
	}

	int ShaderProgram::getAttribLocation(const char * name)
	{
		return glGetAttribLocation(m_ID, "position");;
	}
}
