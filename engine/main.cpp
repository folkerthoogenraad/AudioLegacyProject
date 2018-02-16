#include "Win32Window.h"

#include <iostream>

#include "ShaderProgram.h"

#include "GL.h"

int main() {
	using namespace apryx;

	Win32Window window("OpenGL");

	ShaderProgram program(
		Shader(Shader::Vertex, DEFAULT_VERTEX_SOURCE),
		Shader(Shader::Fragment, DEFAULT_FRAGMENT_SOURCE));

	program.use();

	float vertices[] = {
		0.0f,  0.5f, // Vertex 1 (X, Y)
		0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
	};

	GLuint vbo;
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(SHADER_POSITION_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SHADER_POSITION_LOCATION);


	checkGLError();

	window.setVisible(true);

	while (!window.isCloseRequested()) {
		window.poll();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		window.swap();
	}

	window.destroy();
}