#include "engine/Win32Window.h"

#include <iostream>

#include "engine/GLShaderProgram.h"
#include "engine/GLVertexBufferObject.h"

#include "engine/GL.h"

#include "math/Matrix4.h"

#include "engine/Image.h"
#include "engine/GLTexture.h"

#include "engine/Timer.h"


int test() {
	using namespace apryx;

	Win32Window window("OpenGL", 1280, 720, false);

	glClearColor(1, 1, 1, 1);

	Timer timer;
	double timeSum = 0;
	int frameCount = 0;

	GLShaderProgram program(
		GLShader(GLShader::Vertex, VERTEX_DEFAULT_SOURCE),
		GLShader(GLShader::Fragment, FRAGMENT_UNLIT_TEXTURE));

	Image image = Image::checkerboard(16, 16);

	GLTexture texture;
	texture.setFiltering(GLTexture::NearestNeighbour);
	texture.setWrapping(GLTexture::Clamp);

	texture.setData(image);

	texture.bind();

	program.use();

	int matrixModel = program.getUniformLocation(SHADER_MATRIX_MODEL);
	int matrixView = program.getUniformLocation(SHADER_MATRIX_VIEW);
	int matrixProjection = program.getUniformLocation(SHADER_MATRIX_PROJECTION);

	int textureLocation = program.getUniformLocation(SHADER_MAIN_TEXTURE);

	program.setUniform(matrixView, Matrix4f::translation(0, 0, 2));
	program.setUniform(matrixProjection, Matrix4f::perspective(60, 16.f / 9.f, 0.1f, 1000.f));

	program.setUniform(textureLocation, 0);

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

	float uvs[] = {
		0.5f, 1,
		0, 0,
		1, 0,
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


	GLVertexBufferObject uvBuffer;
	uvBuffer.setBufferData(GLVertexBufferObject::Static, sizeof(uvs), uvs);

	uvBuffer.bind();

	glVertexAttribPointer(SHADER_UV_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(SHADER_UV_LOCATION);

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
		Timer::sleep(1 / 60.f);
	}

	window.destroy();

	return 0;
}
