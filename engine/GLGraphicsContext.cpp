#include "GLGraphicsContext.h"

#include "GLTexture.h"
#include "GLGraphics2D.h"

namespace apryx {
	
	std::shared_ptr<Texture> apryx::GLGraphicsContext::createTexture()
	{
		auto texture = std::make_shared<GLTexture>();

		texture->setFiltering(GLTexture::Linear);
		texture->setWrapping(GLTexture::Clamp);

		return texture;
	}

	std::shared_ptr<Graphics2D> GLGraphicsContext::createGraphics()
	{
		return std::make_shared<GLGraphics2D>(shared_from_this());
	}

}
