#include "Win32Window.h"

#include <iostream>

#include "GLShaderProgram.h"
#include "GLVertexBufferObject.h"

#include "GL.h"

#include "math/Matrix4.h"

int main() {
	using namespace apryx;

	Win32Window window("OpenGL");

	GLShaderProgram program(
		GLShader(GLShader::Vertex, DEFAULT_VERTEX_SOURCE),
		GLShader(GLShader::Fragment, DEFAULT_FRAGMENT_SOURCE));

	program.use();

	int l = program.getUniformLocation("matrix");

	program.setUniform(l, Matrix4f::translation(0.5f, 0.5f, 0));

	float vertices[] = {
		0.0f,  0.5f, 0, // Vertex 1 (X, Y)
		0.5f, -0.5f, 0, // Vertex 2 (X, Y)
		-0.5f, -0.5f, 0  // Vertex 3 (X, Y)
	};

	GLVertexBufferObject vbo;
	vbo.setBufferData(GLVertexBufferObject::Static, sizeof(vertices), vertices);
	
	vbo.bind();

	glVertexAttribPointer(SHADER_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SHADER_POSITION_LOCATION);

	checkGLError();

	window.setVisible(true);

	while (!window.isCloseRequested()) {
		window.poll();

		if (window.isResized()) {
			std::cout << "Wat" << std::endl;
			glViewport(0, 0, window.getWidth(), window.getHeight());
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		window.swap();
	}

	window.destroy();
}