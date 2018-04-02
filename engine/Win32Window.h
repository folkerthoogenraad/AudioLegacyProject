#pragma once

#include "Window.h"
#include <Windows.h>

namespace apryx {

	class Win32Window : public Window {
		HWND m_Hwnd;
		HGLRC m_GLContext;

		bool m_CloseRequested;
		bool m_Resized;

		int m_Width;
		int m_Height;

		float m_DPIScale = 1;
	public:
		Win32Window(std::string title, int width = 640, int height = 480, bool full = false);

		void setVisible(bool visible, bool maximize = false);

		bool isResized() const override;
		bool isCloseRequested() const override;

		float getWidth() const override;
		float getHeight() const override;

		int getRawWidth() const { return m_Width; }
		int getRawHeight() const { return m_Height; }

		LRESULT handleWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void destroy() override;

		float dpiScale() const override;

		void poll() override;
		void swap() override;
	};

}