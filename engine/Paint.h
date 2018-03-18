#pragma once

#include "math/Vector4.h"

#include "Font.h"

#include <memory>

namespace apryx {

	class Paint {
	public:
		enum Style{
			Fill,
			Stroke,
			FillAndStroke
		};
	private:
		Color32 m_Color;
		Color32 m_StrokeColor;
		Style m_Style;

		std::shared_ptr<Font> m_Font;
	public:
		void setColor(Color32 c) { m_Color = c; }
		Color32 getColor() const { return m_Color; }

		void setStrokeColor(Color32 c) { m_StrokeColor = c; }
		Color32 getStrokeColor() const { return m_StrokeColor; }

		Style getStyle() const { return m_Style; }
		void setStyle(Style style) { m_Style = style; }

		std::shared_ptr<Font> getFont() const { return m_Font; }
		void setFont(std::shared_ptr<Font> font) { m_Font = font; }
	};

}