#pragma once

#include "Texture.h"

namespace apryx {

	class GLTexture : public Texture{
		unsigned int m_ID;
	public:
		enum WrappingMode {
			Clamp,
			Repeat
		};
		enum TextureFiltering {
			Linear,
			NearestNeighbour
		};
	public:
		GLTexture();
		~GLTexture();

		void bind();
		void unbind();

		void setFiltering(TextureFiltering filtering);
		void setWrapping(WrappingMode wrapping);

		// TODO use some caching or something to find the correct width and height yafeelme
		void setData(const Image &image) override;
	};

}