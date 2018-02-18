#pragma once

#include "GLShader.h"

#include "math/Defines.h"

namespace apryx {

	class GLShaderProgram {
	private:
		unsigned int m_ID;

	public:
		GLShaderProgram(const GLShader &vertexShader, const GLShader &fragmentShader);

		void use();
		void reset();

		int getAttribLocation(const char *name);
		int getUniformLocation(const char *name);

		// Only use when shader is bound!
		void setUniform(int index, float v);
		void setUniform(int index, Vector3f v);
		void setUniform(int index, Matrix4f v);
	};


}