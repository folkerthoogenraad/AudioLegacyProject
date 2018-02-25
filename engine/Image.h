#pragma once

#include <vector>
#include "math/Vector4.h"

namespace apryx {

	class Image {
		int m_Width;
		int m_Height;
		std::vector<Color32> m_Colors;
	public:
		Image(int width, int height);

		void setColor(int x, int y, Color32 color);
		Color32 getColor(int x, int y);

		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }

		static Image checkerboard(int width, int height, Color32 a = Color32::white(), Color32 b = Color32::black());
		static Image colored(int width, int height, Color32 color = Color32::white());

		const std::vector<Color32> &getColors() const { return m_Colors; }
	};

}