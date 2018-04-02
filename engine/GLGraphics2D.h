#pragma once

#include "Graphics2D.h"
#include "GLBatch.h"
#include "GLGraphicsContext.h"

namespace apryx {

	class GLGraphics2D : public Graphics2D{
		std::shared_ptr<GLGraphicsContext> m_GraphicsContext;
		std::shared_ptr<GLTexture> m_WhiteTexture;

		GLBatch m_Batch;
	public:
		GLGraphics2D(std::shared_ptr<GLGraphicsContext> ctx);

		virtual void setSize(float width, float height);

		virtual void drawRectangle(Paint &paint, Rectanglef rectangle) override;

		virtual void drawElipse(Paint &paint, Rectanglef rectangle) override;
		virtual void drawRoundedRectangle(Paint &paint, Rectanglef rectangle, Rounding rounding) override;

		virtual void drawArc(Paint &paint, Vector2f center, float radius, float startAngle, float sweepAngle) override;

		virtual void drawText(Paint &paint, Vector2f pos, std::string text) override;

		virtual void drawImage(Paint &paint, Image &image, Vector2f pos) override;
		virtual void drawImage(Paint &paint, Image &image, Rectanglef rectangle) override;

		virtual void flush();
	};

}