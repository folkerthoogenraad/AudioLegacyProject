#include "SpriteBatch.h"

namespace apryx {

	SpriteBatch::SpriteBatch()
		: m_Program(
			GLShader(GLShader::Vertex, VERTEX_DEFAULT_SOURCE),
			GLShader(GLShader::Fragment, FRAGMENT_UNLIT_TEXTURE))
	{
	}

	void SpriteBatch::begin()
	{
		if (m_Drawing)
			return;

		m_Index = 0;
	}
	void SpriteBatch::end()
	{
		if (!m_Drawing)
			return;

		flush();
		m_Drawing = false;
	}
	void SpriteBatch::flush()
	{
		if (!m_Drawing) {
			m_Index = 0; // This is actually a bit of a hack
			return;
		}
	}

	void SpriteBatch::texture(std::shared_ptr<GLTexture> texture)
	{
		if (texture == m_Texture)
			return;

		m_Texture = texture;
		
		flush();
	}

	void SpriteBatch::vertex(Vector3f vertex)
	{
		m_CurrentVertex.vertex = vertex;

		m_Vertices[m_Index] = m_CurrentVertex;
		
		m_Index++;

		if (m_Index >= m_Vertices.size())
			flush();
	}

	void SpriteBatch::uv(Vector2f uv)
	{
		m_CurrentVertex.uv = uv;
	}
	void SpriteBatch::color(Color32 color)
	{
		m_CurrentVertex.color = color;
	}
}
