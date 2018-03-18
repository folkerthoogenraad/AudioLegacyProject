#pragma once

#include <array>
#include <memory>

#include "math/Vector4.h"	// Color32

#include "math/Vector3.h"	// Vector3f
#include "math/Vector2.h"	// Vector2f

#include "GLVertexBufferObject.h"
#include "GLShaderProgram.h"
#include "GLTexture.h"

namespace apryx {

	class SpriteBatch {
	public:
		struct Vertex {
			Vector3f vertex;
			Vector2f uv;
			Color32 color;
		};

	private:
		bool m_Drawing = false;
		int m_Index = 0;
		std::array<Vertex, 1024> m_Vertices;
		Vertex m_CurrentVertex;

		std::shared_ptr<GLTexture> m_Texture;

		GLShaderProgram m_Program;
		GLVertexBufferObject m_VertexBufferObject;
	public:
		SpriteBatch();

		void begin();
		void end();
		void flush();

		void vertex(Vector3f vertex);
		void uv(Vector2f uv);
		void color(Color32 color);
		
		void texture(std::shared_ptr<GLTexture> texture);

		bool isDrawing() const { return m_Drawing; }
	};

}