#include "Win32Window.h"

#include <iostream>

#include "GLShaderProgram.h"
#include "GLVertexBufferObject.h"

#include "GL.h"

#include "math/Matrix4.h"

#include "Timer.h"

int main() {
	using namespace apryx;

	Win32Window window("OpenGL", 1280, 720, false);
	
	glClearColor(0, 0, 0, 1);

	Timer timer;
	double timeSum = 0;
	int frameCount = 0;

	GLShaderProgram program(
		GLShader(GLShader::Vertex, VERTEX_DEFAULT_SOURCE),
		GLShader(GLShader::Fragment, FRAGMENT_UNLIT_COLOR));


	program.use();

	int matrixModel = program.getUniformLocation(SHADER_MATRIX_MODEL);
	int matrixView = program.getUniformLocation(SHADER_MATRIX_VIEW);
	int matrixProjection = program.getUniformLocation(SHADER_MATRIX_PROJECTION);

	program.setUniform(matrixView, Matrix4f::translation(0, 0, 4));
	program.setUniform(matrixProjection, Matrix4f::perspective(60, 16.f/9.f, 0.1f, 1000.f));

	float vertices[] = {
		0.0f,  0.5f, 0, // Vertex 1 (X, Y)
		0.5f, -0.5f, 0, // Vertex 2 (X, Y)
		-0.5f, -0.5f, 0  // Vertex 3 (X, Y)
	};

	float colors[] = {
		1,0,0,1,
		0,1,0,1,
		0,0,1,1
	};

	GLVertexBufferObject vertexBuffer;
	vertexBuffer.setBufferData(GLVertexBufferObject::Static, sizeof(vertices), vertices);
	
	vertexBuffer.bind();

	glVertexAttribPointer(SHADER_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SHADER_POSITION_LOCATION);


	GLVertexBufferObject colorBuffer;
	colorBuffer.setBufferData(GLVertexBufferObject::Static, sizeof(colors), colors);

	colorBuffer.bind();

	glVertexAttribPointer(SHADER_COLOR_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SHADER_COLOR_LOCATION);

	checkGLError();

	window.setVisible(true);

	timer.start();

	while (!window.isCloseRequested()) {
		window.poll();

		if (window.isResized()) {
			glViewport(0, 0, window.getWidth(), window.getHeight());
		}

		program.setUniform(matrixModel, Matrix4f::rotationY(timeSum * 360));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		timeSum += timer.lap();
		frameCount++;
		if (timeSum > 1) {
			std::cout << "FPS : " << frameCount << std::endl;
			frameCount = 0;
			timeSum -= 1;
		}

		window.swap();
	}

	window.destroy();
}