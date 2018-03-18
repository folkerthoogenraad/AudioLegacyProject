#pragma once

#include <string>
#include <array>

#include "Paint.h"
#include "Image.h"

#include "math/Vector2.h"
#include "math/Rectangle.h"

namespace apryx {

	typedef std::array<float, 4> Rounding;

	class Graphics2D {

	public:
		// Implementable interface
		virtual void drawElipse(Paint &paint, Rectanglef &rectangle) = 0;
		virtual void drawRectangle(Paint &paint, Rectanglef &rectangle) = 0;
		virtual void drawRoundedRectangle(Paint &paint, Rectanglef &rectangle, Rounding rounding) = 0;

		virtual void drawArc(Paint &paint, Vector2f center, float radius, float startAngle, float sweepAngle) = 0;

		virtual void drawText(Paint &paint, Vector2f &pos, std::string text) = 0;

		virtual void drawImage(Paint &paint, Image &image) = 0;
		virtual void drawImage(Paint &paint, Image &image, Rectanglef &rectangle) = 0;
	};

}