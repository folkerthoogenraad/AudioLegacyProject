#include "Window.h"

namespace apryx {
	Window::Window(std::string title, int width, int height, bool full)
		: m_Fullscreen(full), m_Title(title)
	{ }
}
