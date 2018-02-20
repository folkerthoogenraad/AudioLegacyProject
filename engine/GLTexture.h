#pragma once

#include "Image.h"

namespace apryx {

	class GLTexture {
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

		void setData(const Image &image);
	};

}