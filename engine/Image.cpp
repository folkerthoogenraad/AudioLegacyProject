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
	Image Image::checkerboard(int width, int height, Color32 a, Color32 b)
	{
		Image image(width, height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Color32 r = a;
				if ((x + y) % 2 == 1) {
					r = b;
				}
				image.setColor(x, y, r);
			}
		}

		return image;
	}
	Image Image::colored(int width, int height, Color32 color)
	{
		Image image(width, height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				image.setColor(x, y, color);
			}
		}

		return image;
	}
}
