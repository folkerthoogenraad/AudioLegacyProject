#include "Shader.h"

#include <iostream>

#include "GL.h"

namespace apryx {
	Shader::Shader(Type type, const char * source)
		: m_Type(type)
	{
		m_ID = glCreateShader(type == Type::Fragment ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);

		glShaderSource(m_ID, 1, &source, 0);
		glCompileShader(m_ID);

		GLint status;
		glGetShaderiv(m_ID, GL_COMPILE_STATUS, &status);

		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(m_ID, 512, NULL, buffer);

			std::cerr << "Failed to compile shader : " << std::endl;
			std::cerr << buffer << std::endl;
		}

		checkGLError();
	}
}
