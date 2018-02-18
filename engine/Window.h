#pragma once

#include <string>

namespace apryx {
	class Window {
	protected:
		int m_Width;
		int m_Height;
		std::string m_Title;
		bool m_Fullscreen;
	public:
		Window(std::string title, int width = 640, int height = 480, bool full = false);

		virtual void destroy() = 0;
		virtual void poll() = 0;
		virtual void swap() = 0;

		virtual bool isCloseRequested() const = 0;
		virtual bool isResized() const = 0;

		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
	};
}