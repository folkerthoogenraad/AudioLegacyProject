#include "GLGraphics2D.h"

#include "Image.h"


namespace apryx {

	GLGraphics2D::GLGraphics2D(std::shared_ptr<GLGraphicsContext> ctx)
		: m_GraphicsContext(ctx)
	{
		Image image = Image::colored(1, 1);
		image.upload(*ctx);

		m_WhiteTexture = std::static_pointer_cast<GLTexture>(image.getTexture());

		m_Batch.texture(m_WhiteTexture);
	}

	void GLGraphics2D::setSize(float width, float height)
	{
		m_Batch.setSize(width, height);
	}

	void GLGraphics2D::drawRectangle(Paint & paint, Rectanglef rectangle)
	{
		// In case we are not drawing yet
		m_Batch.begin();

		m_Batch.texture(m_WhiteTexture);

		m_Batch.color(paint.getColor());

		m_Batch.vertex(rectangle.topleft());
		m_Batch.vertex(rectangle.topright());
		m_Batch.vertex(rectangle.bottomright());
		m_Batch.vertex(rectangle.bottomleft());
	}
	void GLGraphics2D::drawElipse(Paint & paint, Rectanglef rectangle)
	{
	}
	void GLGraphics2D::drawRoundedRectangle(Paint & paint, Rectanglef rectangle, Rounding rounding)
	{
	}
	void GLGraphics2D::drawArc(Paint & paint, Vector2f center, float radius, float startAngle, float sweepAngle)
	{
	}
	void GLGraphics2D::drawText(Paint & paint, Vector2f pos, std::string text)
	{
	}

	void GLGraphics2D::drawImage(Paint & paint, Image & image, Vector2f pos)
	{
		drawImage(paint, image, Rectanglef(pos, Vector2f(image.getWidth(), image.getHeight())));
	}

	void GLGraphics2D::drawImage(Paint & paint, Image & image, Rectanglef rectangle)
	{
		// In case we are not drawing yet
		m_Batch.begin();

		// Upload if need be
		if (image.isDirty())
			image.upload(*m_GraphicsContext);

		m_Batch.texture(std::static_pointer_cast<GLTexture>(image.getTexture()));

		m_Batch.color(paint.getColor());

		m_Batch.uv(Vector2f(0, 0));
		m_Batch.vertex(rectangle.topleft());
		m_Batch.uv(Vector2f(1, 0));
		m_Batch.vertex(rectangle.topright());
		m_Batch.uv(Vector2f(1, 1));
		m_Batch.vertex(rectangle.bottomright());
		m_Batch.uv(Vector2f(0, 1));
		m_Batch.vertex(rectangle.bottomleft());
	}
	void GLGraphics2D::flush()
	{
		m_Batch.end();
	}
}
