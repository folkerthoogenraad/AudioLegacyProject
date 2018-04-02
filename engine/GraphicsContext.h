#pragma once


#include <map>
#include <memory>


namespace apryx {

	class Texture;
	class Graphics2D;

	class GraphicsContext {

	public:
		virtual std::shared_ptr<Texture> createTexture() = 0;
		virtual std::shared_ptr<Graphics2D> createGraphics() = 0;
	};

}