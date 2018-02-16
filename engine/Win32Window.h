#pragma once

#include "Window.h"
#include <Windows.h>

namespace apryx {

	class Win32Window : public Window {
		HWND m_Hwnd;
		HGLRC m_GLContext;

		bool m_CloseRequested;
	public:
		Win32Window(std::string title, int width = 640, int height = 480, bool full = false);

		void setVisible(bool visible);

		bool isCloseRequested() const;

		void destroy();

		void poll();
		void swap();
	};

}