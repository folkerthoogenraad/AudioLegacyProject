#pragma once

#include "GraphicsContext.h"

namespace apryx {

	class GLGraphicsContext : public GraphicsContext, public std::enable_shared_from_this<GLGraphicsContext> {

	public:
		virtual std::shared_ptr<Texture> createTexture();
		virtual std::shared_ptr<Graphics2D> createGraphics();
	};

}