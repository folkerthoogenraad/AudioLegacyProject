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

		void destroy() = delete;
		void poll() = delete;
		void swap() = delete;

		bool isCloseRequested() const = delete;

		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
	};
}