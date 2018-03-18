#pragma once

namespace apryx {

	class Image;

	class Texture {
	public:
		virtual void setData(const Image &image) = 0;
	};

}