#pragma once


#include <map>
#include <memory>


namespace apryx {

	class Texture;

	class GraphicsContext {

	public:
		virtual std::shared_ptr<Texture> createTexture() = 0;
	};

}