#pragma once

#include "Shader.h"

namespace apryx {

	class ShaderProgram {
	private:
		unsigned int m_ID;

	public:
		ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader);

		void use();
		void reset();

		int getAttribLocation(const char *name);
	};


}