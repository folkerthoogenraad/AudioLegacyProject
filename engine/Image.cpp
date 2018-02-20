#include "Image.h"

namespace apryx {
	Image::Image(int width, int height)
		: m_Width(width), m_Height(height)
	{
		m_Colors.resize(m_Width * m_Height);
	}

	void Image::setColor(int x, int y, Color32 color)
	{
		if (x < 0 || x > m_Width || y < 0 || y > m_Height)
			return;

		m_Colors[x + y * m_Width] = color;
	}
	Color32 Image::getColor(int x, int y)
	{
		if (x < 0 || x > m_Width || y < 0 || y > m_Height)
			return Color32(0, 0, 0, 1);

		return m_Colors[x + y * m_Width];
	}
}
