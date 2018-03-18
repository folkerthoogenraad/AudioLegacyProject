#pragma once

#include <vector>
#include <memory>

#include "GraphicsContext.h"
#include "math/Vector4.h"

namespace apryx {

	// TODO create some kind of APITexture reference thing, that later can be used from all different API's
	class GLTexture;

	class Image {
		int m_Width;
		int m_Height;
		std::vector<Color32> m_Colors;

		std::shared_ptr<GLTexture> m_Texture;
	public:
		Image(int width, int height);

		void setColor(int x, int y, Color32 color);
		Color32 getColor(int x, int y);

		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }

		// Maybe give it a GraphicsContext here? 
		// So that it knows how to upload and to what API
		void upload(const GraphicsContext &context);

		std::shared_ptr<GLTexture> getTexture() const { return m_Texture; }

		static Image checkerboard(int width, int height, Color32 a = Color32::white(), Color32 b = Color32::black());
		static Image colored(int width, int height, Color32 color = Color32::white());

		const std::vector<Color32> &getColors() const { return m_Colors; }
	};

}