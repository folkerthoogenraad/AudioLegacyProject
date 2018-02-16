#include "Window.h"

namespace apryx {
	Window::Window(std::string title, int width, int height, bool full)
		:m_Width(width), m_Height(height), m_Fullscreen(full), m_Title(title)
	{ }
}
