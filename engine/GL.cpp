#include "GL.h"

#include <iostream>

bool checkGLError()
{
	// I need more of this error checking like, everywhere
	GLenum error = glGetError();
	if (error == GL_NO_ERROR)
		return false;

	std::cout << "glGetError(" << error << ") : ";


	switch (error) 
	{
	case GL_INVALID_ENUM:
		std::cout << "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		std::cout << "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		std::cout << "GL_INVALID_OPERATION";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "GL_OUT_OF_MEMORY";
		break;

	case GL_STACK_UNDERFLOW:
		std::cout << "GL_STACK_UNDERFLOW";
		break;
	case GL_STACK_OVERFLOW:
		std::cout << "GL_STACK_OVERFLOW";
		break;
	default:
		std::cout << "UNKNOWN";
	}


	std::cout << std::endl;

	return false;
}
