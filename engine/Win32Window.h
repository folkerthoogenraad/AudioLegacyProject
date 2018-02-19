#pragma once

#include "Window.h"
#include <Windows.h>

namespace apryx {

	class Win32Window : public Window {
		HWND m_Hwnd;
		HGLRC m_GLContext;

		bool m_CloseRequested;
		bool m_Resized;

		float m_DPIScale = 1;
	public:
		Win32Window(std::string title, int width = 640, int height = 480, bool full = false);

		void setVisible(bool visible);

		bool isResized() const override;
		bool isCloseRequested() const override;

		LRESULT handleWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void destroy() override;

		float dpiScale() const override;

		void poll() override;
		void swap() override;
	};

}